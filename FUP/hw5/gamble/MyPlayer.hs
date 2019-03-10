import SedmaBase
import SedmaDecks
import SedmaReplay
import SedmaGamble
import Data.Maybe

data MyState = MyState {
	me :: Player,
	myHand :: Hand,
	curTrick :: Trick,
	leader :: Player,
	history :: [Card]
} deriving (Eq, Show)

removeCard :: Hand -> Card -> Hand
removeCard [] oldCard = []
removeCard (tmpCard:oldHand) oldCard | (tmpCard == oldCard) = removeCard oldHand oldCard
								     | otherwise = tmpCard : (removeCard oldHand oldCard)

updateHand :: Hand -> Card -> (Maybe Card) -> Hand
updateHand oldHand oldCard Nothing = (removeCard oldHand oldCard)
updateHand oldHand oldCard (Just newCard) = newCard : (removeCard oldHand oldCard)

updateHistory :: [Card] -> [Card] -> [Card]
updateHistory [] history = history
updateHistory (x:xs) history = (updateHistory xs (x:history))

instance PlayerState MyState where
	initState me hand = MyState me hand [] me []
	updateState trick leader card newCard s = MyState (me s) (updateHand (myHand s) card newCard) trick leader (updateHistory trick (history s))

countOccurrences :: Card -> [Card] -> Int -> Int
countOccurrences _ [] count = count
countOccurrences (Card suit1 rank1) ((Card suit2 rank2):history) count | (rank1 == rank2) = countOccurrences (Card suit1 rank1) history (count + 1)
															  		   | otherwise = (countOccurrences (Card suit1 rank1) history count)

countAllOccurrences :: [Card] -> [Card] -> [Int]
countAllOccurrences [] _ = []
countAllOccurrences (x:xs) history = ((countOccurrences x history 0) : (countAllOccurrences xs history))

countNeutralOccurrences :: [Card] -> [Card] -> [Int]
countNeutralOccurrences [] _ = []
countNeutralOccurrences ((Card suit r):xs) history | ((r /= R10) && (r /= RA) && (r /= R7)) = ((countOccurrences (Card suit r) history 0) : (countAllOccurrences xs history))
									   			   | otherwise = (0 : (countAllOccurrences xs history))

findMaxIndex :: Int -> Int -> [Int] -> Int
findMaxIndex _ maxIdx [] = maxIdx
findMaxIndex idx maxIdx (x:xs) | (x > maxIdx) = findMaxIndex (idx + 1) idx xs
							   | otherwise = findMaxIndex (idx + 1) maxIdx xs

bestNeutral :: [Card] -> [Card] -> Card
bestNeutral hand history = hand !! (findMaxIndex 0 0 (countNeutralOccurrences hand history))

tryToMatch :: Card -> [Card] -> (Maybe Card)
tryToMatch _ [] = Nothing
tryToMatch (Card suit1 r1) ((Card suit2 r2):xs) | (r1==r2) = Just (Card suit2 r2)
												| otherwise = tryToMatch (Card suit1 r1) xs

try7 :: [Card] -> [Card] -> [Card] -> Card
try7 [] hand history = bestNeutral hand history
try7 ((Card suit r):xs) hand history | (r==R7) = (Card suit r)
						 	 		 | otherwise = try7 xs hand history

try10orA :: [Card] -> [Card] -> [Card] -> Card
try10orA [] hand history = bestNeutral hand history
try10orA ((Card suit r):xs) hand history | ((r==R10) || (r==RA)) = (Card suit r)
							 	 		 | otherwise = try10orA xs hand history

tryToBeat :: Card -> [Card] -> [Card] -> Card
tryToBeat card hand history | (match /= Nothing) = (fromJust match)
							| otherwise = try7 hand hand history
							where 
								match = tryToMatch card hand



pickCard :: Trick -> MyState -> Card
pickCard [] s = bestNeutral (myHand s) (updateHistory (myHand s) (history s))
pickCard (card1:[]) s  | ((value card1) == 10) = tryToBeat card1 (myHand s) hist
					   | (match /= Nothing) = (fromJust match)
					   | otherwise = bestNeutral (myHand s) hist
					   where 
						   match = tryToMatch card1 (myHand s)
						   hist = (updateHistory (myHand s) (updateHistory (card1:[]) (history s)))

pickCard (card1:card2:[]) s | ((beats card2 card1) && ((value card1) == 10)) = tryToBeat card1 (myHand s) hist
							| ((beats card2 card1) && (match /= Nothing)) = (fromJust match)
							| (beats card2 card1) = bestNeutral (myHand s) hist
							| (((countOccurrences card1 hist 0) + (countOccurrences (Card Spade R7) hist 0)) > 6) = try10orA (myHand s) (myHand s) hist
						    | otherwise = bestNeutral (myHand s) hist
						    where
								match = tryToMatch card1 (myHand s)
								hist = (updateHistory (myHand s) (updateHistory (card1:card2:[]) (history s)))

							
pickCard (card1:card2:card3:[]) s | ((beats card2 card1) && (not (beats card3 card1))) = try10orA (myHand s) (myHand s) hist
								  | (((beats card3 card1) || (not (beats card2 card1))) && ((value card1) == 0) && ((value card2) == 0) && ((value card3) == 0) && (match /= Nothing)) = (fromJust match) 
								  | (((beats card3 card1) || (not (beats card2 card1))) && (((value card1) == 10) || ((value card2) == 10) || ((value card3) == 10))) = tryToBeat card1 (myHand s) hist
								  | otherwise = bestNeutral (myHand s) hist
								  where
									  match = tryToMatch card1 (myHand s)
									  hist = (updateHistory (myHand s) (updateHistory (card1:card2:card3:[]) (history s)))





player :: AIPlayer MyState
player trick s = pickCard trick s

sillyPlayer :: AIPlayer MyState
sillyPlayer trick s = head (myHand s)
