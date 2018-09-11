import Primes
import Text.Printf
import System.CPUTime
import System.IO

test :: (() -> IO ()) -> IO ()
test f = do
    start <- getCPUTime
    f ()
    end <- getCPUTime

    let diff = (fromIntegral (end - start)) / (10^12)
    printf "%0.3f s\n" (diff :: Float)

writeNPrimesToFile :: Int -> IO ()
writeNPrimesToFile n = writeFile "output" $ show (take n primes)

main :: IO ()
main = do
    test $ \() -> writeNPrimesToFile 100000
    -- test $ \() -> writeNPrimesToFile 1000000