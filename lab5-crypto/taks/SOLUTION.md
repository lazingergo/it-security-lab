## Taks 1

Run:
```bash
python aes_cbc.py -e -p adishamir -i test_plaintext_1.txt -o test_ciphertext_1.crypted
```

Output:
```
Encrypting...Done.
Your solution to Challenge 1: 3b256fb4674da7a88ec2eebae5d79d43
Hint: The correct solution starts with 3b256f.
```

## Task 2
Run:
```bash
python aes_cbc.py -d -p ronrivest -i test_ciphertext_2.crypted -o test_plaintext_2.txt
```

Output:
```
Decrypting...Done.
Your solution to Challenge 2: 537475786e65740a3d3d3d3d3d3d3d0a
Hint: The correct solution starts with 537475.
```

## Task 3
Run:
```bash
python hybrid.py -e -p test_pubkey.pem -i test_plaintext_3.txt -o test_ciphertext_3.txt
```

Output:
```
Encrypting...
Done.
Your solution to Challenge 3: a0a0a9392d9b8bebb272f9691f4c8b6c
Hint: The correct solution starts with a0a0a9.
```

## Task 4
Run:
```bash
python hybrid.py -d -s test_keypair.pem -p test_pubkey.pem -i test_ciphertext_4.crypted -o test_plaintext_4.txt
```

Output:
```
ecrypting...
Signature verification is successful.
Enter a passphrase to decode the saved private key: `crysys`
Done.
Your solution to Challenge 4: 6f66207468652073656374696f6e732e
Hint: The correct solution starts with 6f6620.
```