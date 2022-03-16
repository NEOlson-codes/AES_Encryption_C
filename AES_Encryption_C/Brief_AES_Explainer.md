BRIEF AES EXPLAINER
Written by: NEOlson

This is a whirlwind overview of AES encryption. AES stands for "Advanced Encryption Standard", and it was created by the American NSA. If you want a more comprehensive look into the algorithm, go take a look at NIST FIPS 197 (the standard document that defines it). 


ENCRYPTION MATH (BACKGROUND)
----------------------------
The AES encryption algorithm uses a mathematical framework called "finite fields theory". In this framework, 
binary numbers are treated as polynomials where all coefficients are either '1' or '0'. For example, the binary number '10111011' would be: 

  1      0      1     1     1      0      1    1

 x^7 + 0*x^6 + x^5 + x^4 + x^3 + 0*x^2 + x^1 + 1

So, the maximum polynomial order of a byte is 7. Under this framework, there are several acceptable operations. To add two finite fields polynomials, perform the normal polynomial addition and then reduce each coefficient modulo 2. This allows the number to keep its binary representation. The equivalent binary operation is simply an XOR of the two numbers. A useful feature is that subtraction of two polynomials is the exact same operation, XOR, making addition easily reversible. 

Probably the most useful aspect of using finite fields is that all the operations are reversible. Another useful aspect is that these computations aren't trivial to perform, but they're also not too costly (good for not having your encryption broken and being able to decipher the data when required). 

Multiplication of two finite fields polynomials occurs like regular polynomial multiplication, but all results are reduced by a special polynomial of order 8. In this case, it's (x^8 + x^4 + x^3 + x + 1). It turns out that polynomial multiplication by this method is also reversible. 

A very handy trick is that multiplying two polynomials can be simplified into multiplying 1 polynomial many times by X. 

Think of multiplying two polynomials A and B where A is (x^3 + x + 1) and B is (x^5 + x^2 + 1). It can be broken up as follows: 

(x^3 + x + 1)*B --> (x^3 * B) + (x * B) + B --> x*x*x*B + x*B + B

A finite field 'x' is binary '2'. Multiplying by 2 is extremely easy on a computer, it's just a left bit shift! Also, the modulo (by the polynomial showed above) is applied whenever the order gets to X^8. However, since the order isn't any higher than 8, the modulo is just a single subtraction by ('100011011': the binary representation of the polynomial showed above). As explained earlier subtraction is just an XOR. So, we've broken down a polynomial multiplication that would normally be difficult to do into successive left shifts and XOR's. Awesome!

Part of the beauty of this algorithm is that the mathematics is really optimized to be done on a computer. Thinking about binary numbers as finite fields polynomials provides lots of properties that are useful for decryption (i.e. reversibility) while allowing the computer to efficiently process all the steps. 

Another cool thing about using finite fields here is that binary words (collections of 4 bytes) can also be viewed as a polynomial. However, this time the coefficients are binary bytes. So a 32-bit word could be represented as: 

a1*x^3 + a2*x^2 + a3*x + a4

where the word (MSB to LSB) is [a1, a2, a3, a4]. The mathematical operations are similar too, except multiplication and addition of two polynomials will involve multiplying or adding their coefficients together (as bytes, which we already covered). The multiplication modulo number for words (to keep multiplications from overflowing) is (x^4 + 1). If you want to read more about finite fields operations on words, go read the standard. 



THE ALGORITHM
--------------
This algorithm performs encryption/decryption on a 128-bit block of data. Cipher key lengths can be 128, 192, or 256 bits. Longer cipher keys create a more secure encryption for the input data, but it's computationally more difficult to perform. 

The algorithm is meant to accomplish a few things: 
1. Thoroughly "scramble" the input data so there's no 1-to-1 matches between characters (sufficiently random result).
2. Use operations that are reversible but have a high enough cost that they're hard to do without the cipher key. 
3. Get an output that is the same length as the input (it's a "symmetric" cipher). 

So, we saw in the math section that the finite fields operations (multiplication, division) are reversible. The AES algorithm uses a few other reversible operations to fill out the operation set. Those include circular row shifting on a square 2D matrix and using a look-up table to swap out byte values. The benefit of all of these is that they're very fast to do on a computer. 

In order to "scramble" the data enough, all the reversible operations just mentioned are going to need to be done many times. That's why the encryption/decryption process takes place as a series of rounds doing the same (for the most part) operations. The number of rounds depends on the length of the cipher key. In each round, the operations are taking place on a 128-bit value called the "state" vector. The "state" starts off in round 0 as the 128-bit block that you want to cipher/decipher. Over successive rounds, the 128 bit state vector is transformed again and again towards its final encrypted value. 

Ok, you're probably thinking, "how does the cipher key play into all of this?" Well, here it is. The cipher key is transformed into a longer set of 4-word (128-bit) values called "round keys". The set of these round keys (used for the whole cipher/decipher process) is called the "key schedule". Depending on the length of the cipher key, there will be more or fewer rounds. For a 256-bit cipher key, there are 15 rounds of operations. For a 128-bit key, there are 11 rounds. So, for encryption with a 256-bit cipher key, there are 15 "round keys" in the key schedule. At the end of each round the round key is added (via finite fields math) to the state vector.

So, given all that background, here's a pseudocode of the whole encryption algorithm. What the functions do is provided below the pseudocode. 

/*--------------------------------------------------------------------------------*\
read input_data, cipher_key

state = input_data

key_schedule = create_key_schedule(cipher_key)

round = 0
add_round_key_to_state(state, key_schedule, round) // "Round 0" is only one operation, which is adding the first round key to the state. 

round = 1
while(round < total_rounds)
	apply_substitution_box(state)

	shift_rows(state)

	mix_columns(state)

	add_round_key_to_state(state, key_schedule, round)

	round = round + 1

// The final round is special. The mix_columns function is omitted. 

apply_substitution_box(state)

shift_rows(state)

add_round_key_to_state(state, key_schedule, last_round)

encrypted_data_out = state

/*--------------------------------------------------------------------------------*\

I'll only give a very brief explanation of the functions here. Go either read the standard or my code if you want to get a better understanding of each of the different functions. 

create_key_schedule - Takes the cipher key as input and creates Nr 4-word blocks where Nr is the number of rounds

add_round_key_to_state - Adds (in finite fields math) the round key for the current round of operations to the state

apply_substitution_box - Takes a single byte and substitutes it with a value from a lookup table. An inverse lookup is used for decryption. 

shift_rows - Think of the 128 bits of the state vector as a 4 byte x 4 byte matrix. Circular shift each row of the matrix some number of bytes depending on the row number. 

mix_columns - Mixes up the bytes in each column (again, think of the state as a 4x4 matrix) by performing a set of finite fields operations on each byte that has the other bytes of the column as inputs. 


The decryption process is simply doing the above in reverse, substituting some of the functions for the inverse function. apply_subtitution_box(), shift_rows(), and mix_columns() all have an inverse function. Because "adding" two finite field polynomials is the same thing as "subtracting", add_round_key_to_state() needs no inverse function. It's its own inverse!

I hope you enjoyed learning about AES encryption. Thanks for reading!