> ###"And God said, Let there be light: and there was light."
####- Genesis 1:3

Since the Pope Benedict XVI has sent out his first tweet from his account ([@Pontifex](https://twitter.com/Pontifex)) in 2012, nothing interesting has been done with the Pope's twitterfeed, and so PontiFlux was born.

PontiFlux is an installation that visualises the Pope's twitterfeed in the real world. Whenever the Pope sends out a new Tweet, PontiFlux wil trigger a electro circuit (Arduino) which turns on a bright light in the shape of a latin cross. Taking "Christ shall give thee light" quite literally. 

The initial idea for this project started as a joke. Things got a bit out of hand and well, here it is!

Watch the video made for this project here: [This if PontiFlux on YouTube](http://www.youtube.com/watch?v=j4Djp-W4zPU)


This repository will give you an insight in the Arduino and Web code of the project.

Project made: December 2013




How it works
======
Arduino boot & setup phase
------

1. Arduino boots and a red light is turned on to indicate it's not ready to operate yet

2. The Arduino makes a first GET-request to the server with since_id: 1

   Since_id 1 is the id of the very first tweet ever sent

3. Server recognises the since_id and fetches the twitterfeed of the set user (Pontifex)

   Since all tweets are newer then the very first tweet, the server retreives all tweets from the set user

4. Server filters the feed to only receive the since_id from the very latest tweet

5. Server responds to the Arduino with the id of the most recent Tweet (Example: 398453667667263487) and state 2 within the XML tags `<state></state>` and `<sinceid></sinceid>`

   States are a way of communication used between the server and the Arduino to know what action to perform:  
   1 = No since_id send with the request/error handling  
   2 = Startup phase of the Arduino, send most recent id  
   3 = No new tweet  
   4 = New tweet, unleash Ragnarok

6. Arduino reads the respons from the server, filters the values between the XML tags and puts them into variables

7. The since_id is stored in a variable to act as the memory of the Arduino to use on future requests

8. Arduino recognises state 2 and knows it must now complete the setup phase

9. The red light is turned off and a green light is turned on to indicate it's ready to operate

10. Setup phase is complete



Operation phase
------

1. Every 5 seconds The Arduino makes a first GET-request to the server with the most recent since_id received from the server

2. Server recognises the since_id and fetches a list of tweets send since the provided since_id

    If no new tweets have been sent, the result will be 0

3. If new tweets are available, the server responds to the Arduino with the id of the most recent Tweet (Example: 398453667667263487) and state 4 within the XML tags `<state></state>` and `<sinceid></sinceid>`

    If no new tweets are available, server sends state 3 within the XML tags `<state></state>` to the arduino
    If state 3 is returned, the arduino will do nothing and continue making requests

4. Arduino stores the new since_id

5. Arduino will blink the green led 5 times and turn on the relay

    Any device can be connected to this relay, for this project a bright cross-shaped light

6. Arduino stores the new since_id for future requests

7. Arduino will continue the operation phase
