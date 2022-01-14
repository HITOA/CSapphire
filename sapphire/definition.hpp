#pragma once

#include "operator.hpp"
#include "primitivetype.hpp"
#include "keyword.hpp"

#define BLOCKSTART u8'{'
#define BLOCKEND   u8'}'

/*
Std type of sapphire

signed integer :
	i8			8 bit  / 1 byte		signed
	i16			16 bit / 2 byte		signed
	i32			32 bit / 4 byte		signed
	i64			64 bit / 8 byte		signed

unsigned integer :
	byte		8 bit  / 1 byte		unsigned
	u16			16 bit / 2 byte		unsigned
	u32			32 bit / 4 byte		unsigned
	u64			64 bit / 8 byte		unsigned

floating-point :
	f32			32 bit / 4 byte
	f64			64 bit / 8 byte

	bool		8 bit  / 1 byte		false == 0x00 | true >= 0x01

*/

/*
Keyword for Sapphire
		
	true		true boolean value
	false		false boolean value

Flow Controll Keyword

	if			if statement
	else		else statement
	elif		elif statement
	for			
	while		

Other
		
	func        declare function
	class		declare class
	struct		declare struct (struct can only contain field)
*/

/*
Operator & Precedence Operator

	! ~ + -		0		unary plus & minus
	* / %		1		multiplu, divide, modulo
	+ -			2		binary plus & minus

*/