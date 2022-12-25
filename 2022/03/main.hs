{-# LANGUAGE ImportQualifiedPost #-}

import Data.Char
import Data.Set (Set)
import Data.Set qualified as Set

item2Priority i
    | 'a' <= i && i <= 'z' = ord i - ord 'a' + 1
    | 'A' <= i && i <= 'Z' = ord i - ord 'A' + 1 + 26

(&) = flip ($)

-- part 1
handleLine1 line = Set.intersection (Set.fromList left) (Set.fromList right)
    where (left, right) = splitAt (length line `div` 2) line

-- part 2
findIntersecting :: Ord a => Set a -> [Set a] -> (Set a, [Set a])
findIntersecting y (x:xs) = if not $ null inter
        then (inter, xs)
        else (x:) <$> findIntersecting y xs
    where inter = Set.intersection x y

findGroup :: Ord a => [Set a] -> (Set a, [Set a])
findGroup (x:xs) = findIntersecting x xs & uncurry findIntersecting

findGroups :: Ord a => [Set a] -> [Set a]
findGroups [] = []
findGroups xs = group : findGroups rest
                where (group, rest) = findGroup xs

main :: IO ()
main = do
    rucksacks <- lines <$> getContents
    -- part 1
    -- print $ sum $ item2Priority . Set.elemAt 0 . handleLine1 <$> rucksacks
    -- part 2
    print . sum . fmap (item2Priority . Set.elemAt 0) . findGroups $ Set.fromList <$> rucksacks
