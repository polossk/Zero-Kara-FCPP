vec_add :: (Int, Int) -> (Int, Int) -> (Int, Int)
vec_add (x1, y1) (x2, y2) = (x1 + x2, y1 + y2)

vec_add' :: Int -> Int -> Int -> Int -> (Int, Int)
vec_add' x1 y1 x2 y2 = (x1 + x2, y1 + y2)

main = do
    putStrLn $ show $ vec_add (1, 2) (3, 4) -- => (4, 6)
    putStrLn $ show $ vec_add' 1 2 3 4      -- => (4, 6)
