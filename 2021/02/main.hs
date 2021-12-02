import Text.Printf

type Instruction = (String, Int)

doInstruction :: Instruction -> (Int, Int, Int) -> (Int, Int, Int)
doInstruction ("forward", n) (x, y, a) = (x + n, y + n * a, a)
doInstruction ("down", n) (x, y, a) = (x, y, a + n)
doInstruction ("up", n) (x, y, a) = (x, y, a - n)

parse :: [String] -> Instruction
parse [s, x] = (s, read x)

main = do
    lines' <- lines <$> getContents
    let instructions = parse . words <$> lines'
    let (x, y, a) = foldl (flip doInstruction) (0, 0, 0) instructions
    putStrLn $ printf "%d x %d = %d" x y (x * y)
