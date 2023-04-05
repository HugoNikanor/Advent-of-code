{-# LANGUAGE TemplateHaskell #-}

-- Challange from
-- https://www.reddit.com/r/adventofcode/comments/128t3c6/puzzle_implement_a_fantasy_computer_to_find_out/

import Data.List (find)
import Control.Lens
import Control.Monad.Reader
import Control.Monad.State.Lazy
import Data.Default

-- Borrowed from extra-1.7.13
whileM :: Monad m => m Bool -> m ()
whileM act = do
    b <- act
    when b $ whileM act

data OP = MOVR
        | MOVV
        | ADD
        | SUB
        | PUSH
        | POP
        | JP
        | JL
        | CALL
        | RET
        | PRINT
        | HALT
        deriving (Show)

instance Enum OP where
    fromEnum MOVR  = 10
    fromEnum MOVV  = 11
    fromEnum ADD   = 20
    fromEnum SUB   = 21
    fromEnum PUSH  = 30
    fromEnum POP   = 31
    fromEnum JP    = 40
    fromEnum JL    = 41
    fromEnum CALL  = 42
    fromEnum RET   = 50
    fromEnum PRINT = 60
    fromEnum HALT  = 255

    toEnum 10  = MOVR
    toEnum 11  = MOVV
    toEnum 20  = ADD
    toEnum 21  = SUB
    toEnum 30  = PUSH
    toEnum 31  = POP
    toEnum 40  = JP
    toEnum 41  = JL
    toEnum 42  = CALL
    toEnum 50  = RET
    toEnum 60  = PRINT
    toEnum 255 = HALT


type Reg = Int
type Addr = Int

data Instruction = MovR Reg Reg
                 | MovV Reg Int
                 | Add  Reg Reg
                 | Sub  Reg Reg
                 | Push Reg
                 | Pop Reg
                 | Jp Addr
                 | Jl Reg Reg Addr
                 | Call Addr
                 | Ret
                 | Print Reg
                 | Halt
                 deriving (Show)

sizeof :: Instruction -> Int
sizeof (MovR _ _ ) = 3
sizeof (MovV _ _ ) = 3
sizeof (Add  _ _ ) = 3
sizeof (Sub  _ _ ) = 3
sizeof (Push _   ) = 2
sizeof (Pop _    ) = 2
sizeof (Jp _     ) = 2
sizeof (Jl _ _ _ ) = 4
sizeof (Call _   ) = 2
sizeof (Ret      ) = 1
sizeof (Print _  ) = 2
sizeof (Halt     ) = 1

operands :: OP -> Int
operands MOVR  = 2
operands MOVV  = 2
operands ADD   = 2
operands SUB   = 2
operands PUSH  = 1
operands POP   = 1
operands JP    = 1
operands JL    = 3
operands CALL  = 1
operands RET   = 0 
operands PRINT = 1
operands HALT  = 0

decode :: OP -> [Int] -> Instruction
decode MOVR  [r1, r2]    = MovR r1 r2
decode MOVV  [r1, v]     = MovV r1 v
decode ADD   [r1, r2]    = Add r1 r2
decode SUB   [r1, r2]    = Sub r1 r2
decode PUSH  [r1]        = Push r1
decode POP   [r1]        = Pop r1
decode JP    [a]         = Jp a
decode JL    [r1, r2, a] = Jl r1 r2 a
decode CALL  [a]         = Call a
decode RET   []          = Ret
decode PRINT [r]         = Print r
decode HALT  []          = Halt

-- Load machine code from an array.
-- First addr is base address of code segment (most likely 0)
loadInstructions :: Addr -> [Int] -> [(Addr, Instruction)]
loadInstructions _ [] = []
loadInstructions idx (x:xs) = let op = toEnum x
                                  opc = operands op 
                                  (ops, t) = splitAt opc xs 
                               in (idx, decode op ops) : loadInstructions (idx + opc + 1) t
                                    

-- Numbers manually written assembly. Not that useful, since labels
-- aren't implemented.
numberInstructions :: Addr -> [Instruction] -> [(Addr, Instruction)]
numberInstructions _ [] = []
numberInstructions adr (x:xs) = (adr, x) : numberInstructions (adr + sizeof x) xs

data CPUState = CPUState { _regs  :: (Reg, Reg, Reg, Reg)
                         , _stk :: [Int]
                         , _pc :: Int }
                         deriving (Show)

instance Default CPUState where
    def = CPUState { _regs = def
                   , _stk  = def
                   , _pc   = def
                   }


makeLenses ''CPUState

data CPU = CPU { _st :: CPUState
               -- Ideally this would be a queue, since that would
               -- allow us to start printing before we are finished
               , _out :: [Int] }
               deriving (Show)

instance Default CPU where
    def = CPU def def

makeLenses ''CPU

reg 0 = st . regs . _1
reg 1 = st . regs . _2
reg 2 = st . regs . _3
reg 3 = st . regs . _4

-- Moves the program counter to the next instruction
next :: Instruction -> State CPU ()
next instr = st . pc += sizeof instr

-- Push value onto CPU stack
push :: Int -> State CPU ()
push v = do
    st . stk %= (v:)

-- Pop value from CPU stack, returning the value
pop :: State CPU Int
pop = do
    v <- head <$> use (st .stk)
    st . stk %= tail
    return v

-- Output number
write :: Int -> State CPU ()
write v = out %= (v:)

-- Executes one instruction, transforming the CPU state. Possibly
-- writes to the output buffer. Return False on HALT
exec :: Instruction -> State CPU Bool
exec i@(MovR rd rs) = do
    v <- use (reg rs)
    reg rd .= v
    next i
    return True

exec i@(MovV rd v)  = do
    reg rd .= v
    next i
    return True

exec i@(Add  rd rs) = do
    v <- use (reg rs)
    reg rd %= (v +)
    next i
    return True

exec i@(Sub  rd rs) = do
    v <- use (reg rs)
    reg rd %= (v -)
    next i
    return True

exec i@(Push rs)    = do
    v <- use (reg rs)
    push v
    next i
    return True

exec i@(Pop rd)     = do
    v <- pop
    reg rd .= v
    next i
    return True

exec (Jp a)         = do
    st . pc .= a
    return True

exec i@(Jl r1 r2 a) = do
    v1 <- use (reg r1)
    v2 <- use (reg r2)
    if v1 < v2
        then st . pc .= a
        else next i
    return True

exec i@(Call a)     = do
    pc' <- use (st . pc)
    push (pc' + sizeof i)
    st . pc .= a
    return True

exec Ret            = do
    pc' <- pop
    st . pc .= pc'
    return True

exec i@(Print r)    = do
    v <- use (reg r)
    write v
    next i
    return True

exec Halt           = return False


-- Given an address, tries to look up the instruction at that address
findInstruction' :: Addr -> ReaderT [(Addr, Instruction)] (State CPU) (Maybe Instruction)
findInstruction' addr = ask >>= return . fmap snd . find ((== addr) . fst)

-- Lookup instruction or die
findInstruction :: Addr -> ReaderT [(Addr, Instruction)] (State CPU) Instruction
findInstruction addr = do
    ins <- findInstruction' addr
    case ins of
        Just instr -> return instr
        Nothing    -> error $ "No instruction at address " ++ show addr


-- Executes the current CPU instruction
-- Returns False if execution should be stopped
stepCPU :: ReaderT [(Addr, Instruction)] (State CPU) Bool
stepCPU = do
    pc' <- lift $ use (st . pc)
    instr <- findInstruction pc'
    lift $ exec instr

--------------------------------------------------

machinecode :: [Int]
machinecode = [11, 0, 10, 42, 6, 255, 30, 0, 11, 0, 0, 11, 1, 1, 11, 3, 1, 60, 1, 10, 2, 0, 20, 2, 1, 60, 2, 10, 0, 1, 10, 1, 2, 11, 2, 1, 20, 3, 2, 31, 2, 30, 2, 41, 3, 2, 19, 31, 0, 50]

asm :: [(Addr, Instruction)]
asm = loadInstructions 0 machinecode

main :: IO ()
main = do
    -- mapM_ print asm
    let cpuSt = execState (runReaderT (whileM stepCPU) asm) def
    print $ reverse $ cpuSt ^. out
