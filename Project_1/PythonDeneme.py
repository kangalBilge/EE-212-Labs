# -*- coding: utf-8 -*-
"""
Created on Thu Nov 30 16:24:34 2023

@author: BilgeKaan
"""
def is_prime(num):
    # Check if a number is prime
    if num < 2:
        return False
    for i in range(2, int(num**0.5) + 1):
        if num % i == 0:
            return False
    return True

def gcd(a, b):
    # Calculate the greatest common divisor using Euclid's algorithm
    while b:
        a, b = b, a % b
    return a

def mod_inverse(a, m):
    # Calculate the modular multiplicative inverse using Extended Euclidean Algorithm (Bezout's Identity)
    m0, x0, x1 = m, 0, 1
    while a > 1:
        q = a // m
        m, a = a % m, m
        x0, x1 = x1 - q * x0, x0
    return x1 + m0 if x1 < 0 else x1

def generate_keypair(bits=12):
    # Generates a key pair for the RSA algorithm
    p = 59
    print("p is =",p)
    q = 61
    print("q is =",q)
    n = p * q
    m = (p - 1) * (q - 1)

    # Choose an integer e such that 1 < e < m and gcd(m, e) = 1
    e = 2
    while gcd(m, e) != 1:
        e += 1

    # Use mod_inverse to find d, the modular multiplicative inverse of e (d * e mod m = 1)
    d = mod_inverse(e, m)

    public_key = (n, e)
    private_key = (n, d)

    return public_key, private_key
def power(x,y,p):
    res = 1
    while y>0:
        if y%2==1:
            res = res * x
        y=y//2
        x=(x**2)
    return res % p
        
        
# Example usage:
p_key, s_key = generate_keypair()
print("Public Key (n, e):", p_key)
print("Private Key (n, d):", s_key)