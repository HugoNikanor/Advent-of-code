{-# LANGUAGE ScopedTypeVariables  #-}

import Control.Lens

data RPS = Rock | Paper | Scissors deriving (Show, Eq)

instance Enum RPS where
    toEnum 1 = Rock
    toEnum 2 = Paper
    toEnum 3 = Scissors
    fromEnum Rock     = 1
    fromEnum Paper    = 2
    fromEnum Scissors = 3
    succ Rock     = Paper
    succ Paper    = Scissors
    succ Scissors = Rock
    pred Paper     = Rock    
    pred Scissors  = Paper   
    pred Rock      = Scissors

parse "A" = Rock
parse "B" = Paper
parse "C" = Scissors

instance Ord RPS where
    Rock     < Paper    = True
    Paper    < Scissors = True
    Scissors < Rock     = True
    a        < b        = False
    -- `a < b = a < succ b` doesn't work since 
    -- `fromEnum (succ b) < fromEnum a` may be true
    a <= b = a < b || a == b

data Result = Win | Draw | Loss deriving (Show, Eq)

parseR "X" = Loss
parseR "Y" = Draw
parseR "Z" = Win

calculateRound :: RPS -> Result -> Int
calculateRound x Win  = 6 + fromEnum (succ x)
calculateRound x Draw = 3 + fromEnum x
calculateRound x Loss = fromEnum (pred x)

toTup2 :: [a] -> (a, a)
toTup2 [a, b] = (a, b)

main :: IO ()
main = do
    input :: [(RPS, Result)] <- fmap ((_2 %~ parseR) . (_1 %~ parse) . toTup2 . words) . lines <$> getContents
    print . sum $ uncurry calculateRound <$> input
