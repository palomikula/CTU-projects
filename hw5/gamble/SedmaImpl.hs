import SedmaBase
import SedmaDecks
import SedmaReplay
import SedmaGamble

data SedmaState = SedmaState {
	actualPlayer :: Player,
	actualHand :: Hand,
	trick :: Trick,
	leader :: Player
} deriving (Eq, Show)

removeCard :: Hand -> Card -> Hand
removeCard [] _ = []
removeCard (x:xs) card 	| x == card = removeCard xs card
						| otherwise = x : removeCard xs card

-- vymaze kartu a prida novu ak ziskal na konci kola
removeAndAddCard :: Hand -> Card -> (Maybe Card) -> Hand
removeAndAddCard hand remCard Nothing = removeCard hand remCard
removeAndAddCard hand remCard (Just card) = (removeCard hand remCard) ++ [card]

instance PlayerState SedmaState where
	initState p h = SedmaState p h [] p
	updateState trick leader card newCard state = SedmaState (actualPlayer state) (removeAndAddCard (actualHand state) card newCard) trick leader

player :: AIPlayer SedmaState
player trick state = getCardBetter trick state

-- glupy ja talpa
getCard :: Trick -> SedmaState -> Card
getCard trick state = head (actualHand state)

-- inteligent ne taky
getCardBetter :: Trick -> SedmaState -> Card
getCardBetter [] state = head (actualHand state) -- emptyTrick state
getCardBetter trick state = getCardByFirstCard trick state -- zatial hlupy

getBetterCard :: Trick -> SedmaState -> Card
getBetterCard trick state = getCardByFirstCard trick state

getCardByFirstCard :: Trick -> SedmaState -> Card
getCardByFirstCard trick state 
	| getElement (actualHand state) (head trick) == Nothing = head (actualHand state)
	| otherwise = getSameCardByFirstCard trick state (getElement (actualHand state) (head trick))

getSameCardByFirstCard :: Trick -> SedmaState -> Maybe Card -> Card
getSameCardByFirstCard trick state (Just card)
	| (getRank card) == RA || (getRank card) == R10 = card
	| (getElement (actualHand state) (Card Heart R7)) /= Nothing = getOnlyCard (getElement (actualHand state) (Card Heart R7))
	| otherwise = card

getOnlyCard:: Maybe Card -> Card
getOnlyCard (Just card) = card

getElement :: Hand -> Card -> Maybe Card
getElement [] _ = Nothing
getElement hand card 
	| getRank (head hand) == getRank card = Just (head hand)
	| otherwise = getElement (tail hand) card

emptyTrick :: SedmaState -> Card
emptyTrick state = getCardForEmptyTrick state

getCardForEmptyTrick :: SedmaState -> Card
getCardForEmptyTrick state 
	| length (actualHand state) == 4 = fullHand (actualHand state)
	| length (actualHand state) == 3 = fullHand (actualHand state)
	| length (actualHand state) == 2 = twoHand (actualHand state)
	| otherwise = head (actualHand state)

fullHand :: Hand -> Card
fullHand hand 
	| (getRank (hand !! 0)) == (getRank (hand !! 1)) = head hand
	| (getRank (hand !! 0)) == (getRank (hand !! 2)) = head hand
	| (getRank (hand !! 0)) == (getRank (hand !! 3)) = head hand
	| (getRank (hand !! 1)) == (getRank (hand !! 2)) = hand !! 1
	| (getRank (hand !! 1)) == (getRank (hand !! 3)) = hand !! 1
	| (getRank (hand !! 2)) == (getRank (hand !! 3)) = hand !! 2
	| (getRank (hand !! 0)) /= R7 && (getRank (hand !! 0)) /= R10 = head hand
	| (getRank (hand !! 1)) /= R7 && (getRank (hand !! 1)) /= R10 = hand !! 1
	| (getRank (hand !! 2)) /= R7 && (getRank (hand !! 2)) /= R10 = hand !! 2 
	| (getRank (hand !! 3)) /= R7 && (getRank (hand !! 3)) /= R10 = hand !! 3
	| otherwise = head hand

threeHand :: Hand -> Card
threeHand hand 
	| (getRank (hand !! 0)) == (getRank (hand !! 1)) = head hand
	| (getRank (hand !! 0)) == (getRank (hand !! 2)) = head hand
	| (getRank (hand !! 1)) == (getRank (hand !! 2)) = hand !! 1
	| (getRank (hand !! 0)) /= R7 && (getRank (hand !! 0)) /= R10 = head hand
	| (getRank (hand !! 1)) /= R7 && (getRank (hand !! 1)) /= R10 = hand !! 1
	| (getRank (hand !! 2)) /= R7 && (getRank (hand !! 2)) /= R10 = hand !! 2
	| otherwise = head hand

twoHand :: Hand -> Card
twoHand hand 
	| (getRank (hand !! 0)) == (getRank (hand !! 1)) = head hand
	| (getRank (hand !! 0)) /= R7 && (getRank (hand !! 0)) /= R10 = head hand
	| (getRank (hand !! 1)) /= R7 && (getRank (hand !! 1)) /= R10 = hand !! 1
	| otherwise = head hand

getRank :: Card -> Rank
getRank (Card _ x) = x


sillyPlayer :: AIPlayer SedmaState
sillyPlayer trick s = last (actualHand s)
