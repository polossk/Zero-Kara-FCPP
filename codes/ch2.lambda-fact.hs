fact :: Int -> Int
fact 0 = 1
fact n = n * fact (n - 1)
main = putStrLn $ show $ fact 5 -- => 120
