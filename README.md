# memenid

## About
**Memenid** is a simple static file server that implements the **gemini** protocol.

## Features

- Static files serving (*that's all you need*)

## Installation
### Prerequirements
Building and running have been tested on linux, other os support is not guaranteed
You have to have openssl libs installed to build the sources

You also should have your pem files (*usually it key.pem and cert.pem*) in the root of the project.
This is required for TLS.


### Process
- `make`
The binary should be located at *./build/out*
