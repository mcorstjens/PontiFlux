> ###"And God said, Let there be light: and there was light."
####- Genesis 1:3

Since the Pope Benedict XVI has sent out his first tweet from his account ([@Pontifex](https://twitter.com/Pontifex)) in 2012, nothing interesting has been done with the Pope's twitterfeed, and so PontiFlux was born.

PontiFlux is an installation that visualises the Pope's twitterfeed in the real world. Whenever the Pope sends out a new Tweet, PontiFlux wil trigger a electro circuit (Arduino) which turns on a bright light in the shape of a latin cross. Taking "Christ shall give thee light" quite literally. 

The initial idea for this project started as a joke. Things got a bit out of hand and well, here it is!

Watch the video made for this project here: [This if PontiFlux on YouTube](http://www.youtube.com/watch?v=j4Djp-W4zPU)


This repository will give you an insight in the Arduino and Web code of the project.

#How it works
1. Arduino boot & setup phase
2. Red light pops up to indicate it's not ready
2. The Arduino makes a GET-request to the server with a dummy since_id (1)
3. Server recognises the dummy since_id and fetches the twitterfeed of the set user
4. Server responds to the Arduino with the id of the most recent Tweet
5. ?
6. ?
7. ?
8. New Tweet incomming
9. Blink green led 5 times


tl;dr
1. Arduino boots up
2. Checks with the server for new tweets
3. New one? BANG! Turns on the lightswitch
