Hamming Simulator
------------------------------------

This application is a proof of concept for simulating and correcting bit errors using Hamming(12,8) single error correcting (SEC). A binary file, such as an image, is read from the filesystem one byte at a time. Each byte is encoded with Hamming(12,8), producing a 12-bit code word. A loop randomly flips the sequence of bits using a uniform random distribution with a probability defined by a global constant. Finally, the code word is decoded and an attempt is made to detect and correct an error. The decoded byte is written into a secondary binary file, and the process repeats itself for every byte in the input file. The final product is a binary file whose bits have been corrected to the algorithm's best abilities.

### Hamming Encoding and Decoding

The process of encoding and decoding boils down to a vector-matrix multiplication. Two Hamming matrices are defined as 2-dimensional constant arrays: generator matrix and parity-check matrix.

### Future Improvements

* Extend for double-error detection (SECDEC) 
* Generalize the application to encode in Hamming(n,k)

### Further Reading

* http://logos.cs.uic.edu/366/notes/ErrorCorrectionAndDetectionSupplement.pdf
* https://en.wikipedia.org/wiki/Hamming(7,4)
* https://www.cs.cmu.edu/~venkatg/teaching/codingtheory/notes/notes1.pdf