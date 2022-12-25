{-# LANGUAGE ScopedTypeVariables #-}

import Data.Maybe
import System.Environment
import Data.List (sort)

parseLine :: String -> Maybe Integer
parseLine "" = Nothing
parseLine ds = Just . read $ ds

swap (a, b) = (b, a)

splitBy1' :: Eq a => a -> [a] -> ([a], [a])
splitBy1' _ [] = ([], [])
splitBy1' k (x:xs)
    | k == x = (xs, [])
    | otherwise = (x:) <$> splitBy1' k xs

splitBy1 :: Eq a => a -> [a] -> ([a], [a])
splitBy1 k xs = swap $ splitBy1' k xs

splitBy :: Eq a => a -> [a] -> [[a]]
splitBy k xs = case splitBy1 k xs of
    ([], []) -> []
    (h, t)   -> h : splitBy k t
    

main :: IO ()
main = do
    args <- getArgs
    let elfCount :: Int = if null args
        then 1
        else read . head $ args
    inp <- lines <$> getContents
    let groups' = splitBy Nothing $ parseLine <$> inp
    let groups :: [[Integer]] = fmap fromJust <$> groups'
    print . sum . take elfCount . reverse . sort $ fmap sum groups
