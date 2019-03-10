import SedmaDatatypes

instance Eq Rank 
    where
        (==) R7 R7 = True
        (==) R8 R8 = True
        (==) R9 R9 = True
        (==) R10 R10 = True
        (==) RJ RJ = True
        (==) RQ RQ = True
        (==) RK RK = True
        (==) RA RA = True
        (==) _ _ = False

evaluateGame 9 8 = (AC,Three)
evaluateGame 0 0 = (BD,Three)
evaluateGame 9 _ = (AC,Two)
evaluateGame 0 _ = (BD, Two)
evaluateGame pointsAC tricksAC | (pointsAC > 4) = (AC, One)
                               | otherwise = (BD, One)

callEvaluateRound ((Card suit rank):xs) ys startingAC pointsAC tricksAC = evaluateRound ys startingAC pointsAC tricksAC 0 1 1 rank

evaluateRound [] startingAC pointsAC tricksAC roundPoints roundWinner _ _ | ((startingAC) && ((roundWinner == 1) || (roundWinner == 3))) = (startingAC, (pointsAC + roundPoints), (tricksAC + 1))
                                                                          | (startingAC) = ((not(startingAC)), pointsAC, tricksAC)
                                                                          | ((roundWinner == 1) || (roundWinner == 3)) = (startingAC, pointsAC, tricksAC) 
                                                                          | otherwise = ((not(startingAC)), (pointsAC + roundPoints), (tricksAC+1))                                                                             

evaluateRound ((Card suit rank):xs) startingAC pointsAC tricksAC roundPoints roundWinner player rankToBeat | (((rank == rankToBeat) || (rank == R7)) && ((rank == R10) || (rank == RA))) = evaluateRound xs startingAC pointsAC tricksAC (roundPoints+1) player (player+1) rankToBeat
                                                                                                           | ((rank == rankToBeat) || (rank == R7)) = evaluateRound xs startingAC pointsAC tricksAC roundPoints player (player+1) rankToBeat
                                                                                                           | ((rank == R10) || (rank == RA)) = evaluateRound xs startingAC pointsAC tricksAC (roundPoints+1) roundWinner (player+1) rankToBeat
                                                                                                           | otherwise = evaluateRound xs startingAC pointsAC tricksAC roundPoints roundWinner (player+1) rankToBeat


                                                                    
play [] startingAC pointsAC tricksAC = evaluateGame finalPointsAC tricksAC
                                            where finalPointsAC = if(startingAC) then (pointsAC+1) else pointsAC
play xs startingAC pointsAC tricksAC = play (drop 4 xs) newStartingAC newPointsAC newTricksAC
                                            where (newStartingAC, newPointsAC, newTricksAC) = callEvaluateRound (take 4 xs) (take 4 xs) startingAC pointsAC tricksAC


replay xs | (length xs) /= 32 = Nothing
          | otherwise = Just (play xs True 0 0)