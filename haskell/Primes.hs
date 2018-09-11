module Primes (

  primes, wheelSieve,

  isPrime, primeFactors

  ) where

primes :: Integral int => [int]
primes = wheelSieve 6

{-# SPECIALISE primes :: [Int]     #-}
{-# SPECIALISE primes :: [Integer] #-}

wheelSieve :: Integral int
          => Int
          -> [int]
wheelSieve k = reverse ps ++ map head (sieve p (cycle ns))
  where (p:ps,ns) = wheel k

{-# SPECIALISE wheelSieve :: Int -> [Int]     #-}
{-# SPECIALISE wheelSieve :: Int -> [Integer] #-}

isPrime :: Integral int => int -> Bool
isPrime n | n > 1     = primeFactors n == [n]
          | otherwise = False

{-# SPECIALISE isPrime :: Int     -> Bool #-}
{-# SPECIALISE isPrime :: Integer -> Bool #-}

primeFactors :: Integral int => int -> [int]
primeFactors n = factors n (wheelSieve 6)
  where
  factors 1 _                  = []
  factors m (p:ps) | m < p*p   = [m]
                  | r == 0    = p : factors q (p:ps)
                  | otherwise = factors m ps
    where (q,r) = quotRem m p

{-# SPECIALISE primeFactors :: Int     -> [Int]     #-}
{-# SPECIALISE primeFactors :: Integer -> [Integer] #-}

sieve :: (Ord int, Num int) => int -> [int] -> [[int]]
sieve p ns@(m:ms) = spin p ns : sieveComps (p+m) ms (composites p ns)

{-# SPECIALISE sieve :: Int     -> [Int]     -> [[Int]]     #-}
{-# SPECIALISE sieve :: Integer -> [Integer] -> [[Integer]] #-}

type Composites int = (Queue int,[[int]])

composites :: (Ord int, Num int) => int -> [int] -> Composites int
composites p ns = (Empty, map comps (spin p ns : sieve p ns))
 where comps xs@(x:_) = map (x*) xs

{-# SPECIALISE composites :: Int     -> [Int]     -> Composites Int     #-}
{-# SPECIALISE composites :: Integer -> [Integer] -> Composites Integer #-}

splitComposites :: Ord int => Composites int -> (int,Composites int)
splitComposites (Empty, xs:xss) = splitComposites (Fork xs [], xss)
splitComposites (queue, xss@((x:xs):yss))
  | x < z     = (x, discard x (enqueue xs queue, yss))
  | otherwise = (z, discard z (enqueue zs queue', xss))
  where (z:zs,queue') = dequeue queue

{-# SPECIALISE splitComposites :: Composites Int -> (Int,Composites Int) #-}
{-# SPECIALISE
    splitComposites :: Composites Integer -> (Integer,Composites Integer) #-}

discard :: Ord int => int -> Composites int -> Composites int
discard n ns | n == m    = discard n ms
             | otherwise = ns
  where (m,ms) = splitComposites ns

{-# SPECIALISE discard :: Int -> Composites Int -> Composites Int #-}
{-# SPECIALISE
    discard :: Integer -> Composites Integer -> Composites Integer #-}

sieveComps :: (Ord int, Num int) => int -> [int] -> Composites int -> [[int]]
sieveComps cand ns@(m:ms) xs
  | cand == comp = sieveComps (cand+m) ms ys
  | cand <  comp = spin cand ns : sieveComps (cand+m) ms xs
  | otherwise    = sieveComps cand ns ys
  where (comp,ys) = splitComposites xs

{-# SPECIALISE sieveComps :: Int -> [Int] -> Composites Int -> [[Int]] #-}
{-# SPECIALISE
    sieveComps :: Integer -> [Integer] -> Composites Integer -> [[Integer]] #-}

spin :: Num int => int -> [int] -> [int]
spin x (y:ys) = x : spin (x+y) ys

{-# SPECIALISE spin :: Int     -> [Int]     -> [Int]     #-}
{-# SPECIALISE spin :: Integer -> [Integer] -> [Integer] #-}

type Wheel int = ([int],[int])

wheel :: Integral int => Int -> Wheel int
wheel n = iterate next ([2],[1]) !! n

{-# SPECIALISE wheel :: Int -> Wheel Int     #-}
{-# SPECIALISE wheel :: Int -> Wheel Integer #-}

next :: Integral int => Wheel int -> Wheel int
next (ps@(p:_),xs) = (py:ps,cancel (product ps) p py ys)
  where (y:ys) = cycle xs
        py = p + y

{-# SPECIALISE next :: Wheel Int     -> Wheel Int     #-}
{-# SPECIALISE next :: Wheel Integer -> Wheel Integer #-}

cancel :: Integral int => int -> int -> int -> [int] -> [int]
cancel 0 _ _ _ = []
cancel m p n (x:ys@(y:zs))
  | nx `mod` p > 0 = x : cancel (m-x) p nx ys
  | otherwise      = cancel m p n (x+y:zs)
  where nx = n + x

{-# SPECIALISE cancel :: Int -> Int -> Int -> [Int] -> [Int] #-}
{-# SPECIALISE
    cancel :: Integer -> Integer -> Integer -> [Integer] -> [Integer] #-}

data Queue int = Empty | Fork [int] [Queue int]

enqueue :: Ord int => [int] -> Queue int -> Queue int
enqueue ns = merge (Fork ns [])

{-# SPECIALISE enqueue :: [Int]     -> Queue Int     -> Queue Int     #-}
{-# SPECIALISE enqueue :: [Integer] -> Queue Integer -> Queue Integer #-}

merge :: Ord int => Queue int -> Queue int -> Queue int
merge Empty y                        = y
merge x     Empty                    = x
merge x     y     | prio x <= prio y = join x y
                  | otherwise        = join y x
  where prio (Fork (n:_) _) = n
        join (Fork ns qs) q = Fork ns (q:qs)

{-# SPECIALISE merge :: Queue Int     -> Queue Int     -> Queue Int     #-}
{-# SPECIALISE merge :: Queue Integer -> Queue Integer -> Queue Integer #-}

dequeue :: Ord int => Queue int -> ([int], Queue int)
dequeue (Fork ns qs) = (ns,mergeAll qs)

{-# SPECIALISE dequeue :: Queue Int     -> ([Int],     Queue Int)     #-}
{-# SPECIALISE dequeue :: Queue Integer -> ([Integer], Queue Integer) #-}

mergeAll :: Ord int => [Queue int] -> Queue int
mergeAll []       = Empty
mergeAll [x]      = x
mergeAll (x:y:qs) = merge (merge x y) (mergeAll qs)

{-# SPECIALISE mergeAll :: [Queue Int]     -> Queue Int     #-}
{-# SPECIALISE mergeAll :: [Queue Integer] -> Queue Integer #-}