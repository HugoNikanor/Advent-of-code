{-# LANGUAGE ScopedTypeVariables  #-}

import Text.ParserCombinators.Parsec
import Control.Lens
import Data.Maybe

parseCrateLine [] = []
parseCrateLine (' ':'[': c :']':xs) = Just c : parseCrateLine xs
parseCrateLine (' ':' ':' ':' ':xs) = Nothing : parseCrateLine xs

type Stack = [Char]

-- handleEntry :: Char -> Int -> State [Stack] ()
-- handleEntry c idx = ix idx %= (c:)
-- 
-- -- if there is something in this spot, add it to the correct stack
-- -- otherwise do nothing
-- buildCrateStacks :: [[Maybe Char]] -> [Stack]
-- buildCrateStacks stks = flip (runState $ replicate (length stks) []) 
--     $ mapM_ (uncurry handleEntry) $ zip line [0..length stks]

transpose :: [[a]] -> [[a]]
transpose [] = []
transpose xs
    | all null xs = []
    | otherwise    =  map head xs : transpose (map tail xs)

swap (a, b) = (b, a)

splitBy1' :: Eq a => a -> [a] -> ([a], [a])
splitBy1' _ [] = ([], [])
splitBy1' k (x:xs)
    | k == x = (xs, [])
    | otherwise = (x:) <$> splitBy1' k xs

splitBy1 :: Eq a => a -> [a] -> ([a], [a])
splitBy1 k xs = swap $ splitBy1' k xs


-- input manually preprocessed:
-- leading space added to initial create configution lines
-- line with create numbers removed

data Move = Move Int Int deriving (Show, Eq)

parseMove :: String -> [Move]
parseMove line = let [_, count, _, from, _, to] = words line
                  in replicate (read count) $ Move (read from - 1) (read to - 1)

doMove :: [Stack] -> Move -> [Stack]
doMove stks (Move from to) = stks & ix to %~ ((head $ stks ^. ix from):)
                                  & ix from %~ tail

main :: IO ()
main = do
    input <- lines <$> getContents
    let (crateLines, moves') = splitBy1 "" input
    let stacks :: [Stack] = fmap (fmap fromJust . filter isJust) $ transpose $ parseCrateLine <$> crateLines
    -- reverse crateLines
    let moves = moves' >>= parseMove
    let result = foldl doMove stacks moves
    print result
    putStrLn $ fmap head result
    -- call function for setting up stacks from createLines
    -- foldl doMove stacks moves
    -- return ()
