import Text.ParserCombinators.Parsec

data Range = Range { start :: Int, end :: Int } deriving (Show, Eq)

number :: GenParser Char () Int
number = read <$> many1 digit

range :: GenParser Char () Range
range = Range <$> number <* char '-' <*> number

line :: GenParser Char () (Range, Range)
line = (,) <$> range <* char ',' <*> range

fullOverlap :: Range -> Range -> Bool
fullOverlap (Range sa ea) (Range sb eb) 
     = sa <= sb && eb <= ea 
    || sb <= sa && ea <= eb

overlaps :: Range -> Range -> Bool
overlaps (Range sa ea) (Range sb eb)
     = sa <= eb && sb <= ea
    || sa <= eb && sb <= ea
    || sa <= sb && eb <= ea
    || sb <= sa && ea <= eb

main :: IO ()
main = do
    input <- getContents
    let Right result = parse (many (line <* char '\n')) "" input
    -- part 1
    -- print . length $ filter (uncurry fullOverlap) result
    -- part 2
    print . length $ filter (uncurry overlaps) result
