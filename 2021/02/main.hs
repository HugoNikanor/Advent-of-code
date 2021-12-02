import Text.Printf

type Instruction = (String, Int)

doInstruction :: Instruction -> (Int, Int) -> (Int, Int)
doInstruction ("forward", n) (x, y) = (x + n, y)
doInstruction ("down", n) (x, y) = (x, y + n)
doInstruction ("up", n) (x, y) = (x, y - n)

parse :: [String] -> Instruction
parse [s, x] = (s, read x)

main = do
    lines' <- lines <$> getContents
    let instructions = parse . words <$> lines'
    let (x, y) = foldl (flip doInstruction) (0, 0) instructions
    putStrLn $ printf "%d x %d = %d" x y (x * y)
