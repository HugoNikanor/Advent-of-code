{-# LANGUAGE ScopedTypeVariables  #-}

data RPS = Rock | Paper | Scissors deriving (Show, Eq)

instance Enum RPS where
    toEnum 1 = Rock
    toEnum 2 = Paper
    toEnum 3 = Scissors
    fromEnum Rock     = 1
    fromEnum Paper    = 2
    fromEnum Scissors = 3

parse "A" = Rock
parse "X" = Rock
parse "B" = Paper
parse "Y" = Paper
parse "C" = Scissors
parse "Z" = Scissors

instance Ord RPS where
    Rock     < Paper    = True
    Paper    < Scissors = True
    Scissors < Rock     = True
    a        < b        = False
    a <= b = a < b || a == b


calculateRound :: [RPS] -> Int
calculateRound [a, x]
    | a < x     = 6 + fromEnum x
    | a == x    = 3 + fromEnum x
    | otherwise = 0 + fromEnum x

main :: IO ()
main = do
    input :: [[RPS]] <- fmap (fmap parse . words) . lines <$> getContents
    print . sum $ calculateRound <$> input
