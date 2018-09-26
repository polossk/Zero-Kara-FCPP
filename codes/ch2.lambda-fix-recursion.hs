fix :: (a -> a) -> a
fix f = let {x = f x} in x
main = putStrLn $ show $ fix (\f n -> if n == 0 then 1 else n * f (n-1)) 5 -- => 120
