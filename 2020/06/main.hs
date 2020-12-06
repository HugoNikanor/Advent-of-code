import qualified Data.Set as Set
import Data.List.Split (splitOn)
import Data.List (lines)

handleGroup group = let sets = Set.fromList <$> group in
    foldl Set.intersection (head sets) (tail sets)

main = do
    groups <- (splitOn [""] . lines) <$> readFile "input"
    putStrLn . show . sum $ length . handleGroup <$> groups
