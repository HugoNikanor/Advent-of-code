{-# LANGUAGE ImportQualifiedPost #-}

import Data.Char
import Data.Set (Set)
import Data.Set qualified as Set

item2Priority i
    | 'a' <= i && i <= 'z' = ord i - ord 'a' + 1
    | 'A' <= i && i <= 'Z' = ord i - ord 'A' + 1 + 26

handleLine line = Set.intersection (Set.fromList left) (Set.fromList right)
    where (left, right) = splitAt (length line `div` 2) line

main :: IO ()
main = do
    rucksacks <- lines <$> getContents
    print $ sum $ item2Priority . Set.elemAt 0 . handleLine <$> rucksacks
