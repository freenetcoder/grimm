
// Copyright (c) 2010-2018 niXman (i dot nixman dog gmail dot com). All
// rights reserved.
//
// This file is part of YAS(https://github.com/niXman/yas) project.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//
//
// Boost Software License - Version 1.0 - August 17th, 2003
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
//
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#ifndef __yas__detail__type_traits__type_traits_hpp
#define __yas__detail__type_traits__type_traits_hpp

#include <yas/detail/config/endian.hpp>
#include <yas/detail/type_traits/has_method_serialize.hpp>
#include <yas/detail/type_traits/has_function_serialize.hpp>
#include <yas/version.hpp>

#include <cstdint>
#include <type_traits>
#include <algorithm>

namespace yas {
namespace detail {

/***************************************************************************/

template<
	 typename T
	,typename A1
	,typename A2 = void
	,typename A3 = void
	,typename A4 = void
	,typename A5 = void
	,typename A6 = void
	,typename A7 = void
	,typename A8 = void
>
struct is_any_of: std::integral_constant<
	bool
	,  std::is_same<T, A1>::value
	|| std::is_same<T, A2>::value
	|| std::is_same<T, A3>::value
	|| std::is_same<T, A4>::value
	|| std::is_same<T, A5>::value
	|| std::is_same<T, A6>::value
	|| std::is_same<T, A7>::value
	|| std::is_same<T, A8>::value
>
{};

/***************************************************************************/

template<typename T>
struct is_array_of_fundamentals
	:std::integral_constant<
		 bool
		,std::is_array<T>::value && std::is_fundamental<typename std::remove_all_extents<T>::type>::value
	>
{};

/***************************************************************************/

template<typename T, typename... Types>
struct enable_if_is_any_of
	:std::enable_if<is_any_of<T, Types...>::value>
{};

template<typename T, typename... Types>
struct disable_if_is_any_of
	:std::enable_if<!is_any_of<T, Types...>::value>
{};

#define __YAS_ENABLE_IF_IS_ANY_OF(T, ...) \
	typename ::yas::detail::enable_if_is_any_of<T, __VA_ARGS__>::type* = 0

#define __YAS_DISABLE_IF_IS_ANY_OF(T, ...) \
	typename ::yas::detail::disable_if_is_any_of<T, __VA_ARGS__>::type* = 0

/***************************************************************************/

enum class type_prop {
	 is_enum
	,is_fundamental
	,is_array
	,is_array_of_fundamentals
	,not_a_fundamental
};

enum class ser_method {
	 has_one_method
	,has_split_methods
	,has_one_function
	,has_split_functions
	,use_internal_serializer
};

template<typename T>
struct type_properties {
	static constexpr type_prop value =
		std::is_enum<T>::value
		? type_prop::is_enum
		: std::is_fundamental<T>::value
			? type_prop::is_fundamental
			: is_array_of_fundamentals<T>::value
				? type_prop::is_array_of_fundamentals
				: std::is_array<T>::value
					? type_prop::is_array
					: type_prop::not_a_fundamental
	;
};

template<typename T, typename Ar>
struct serialization_method {
private:
	enum {
		 is_fundamental = std::is_fundamental<T>::value
		,is_array = std::is_array<T>::value
		,is_enum = std::is_enum<T>::value
	};

public:
	static constexpr ser_method value =
		has_const_method_serializer<is_fundamental || is_array, is_enum, T, void(Ar)>::value
		? ser_method::has_split_methods
		: has_method_serializer<is_fundamental || is_array, is_enum, T, void(Ar)>::value
			? ser_method::has_one_method
			: has_function_const_serialize<is_fundamental || is_array, is_enum, Ar, T>::value
				? ser_method::has_split_functions
				: has_function_serialize<is_fundamental || is_array, is_enum, Ar, T>::value
					? ser_method::has_one_function
					: ser_method::use_internal_serializer
	;
};

/***************************************************************************/

} // namespace detail

/***************************************************************************/

enum options: std::uint32_t {
	 binary    = 1u<<0
	,text      = 1u<<1
	,json      = 1u<<2
	,no_header = 1u<<3
	,elittle   = 1u<<4
	,ebig      = 1u<<5
	,ehost     = 1u<<6
	,compacted = 1u<<7
	,mem       = 1u<<8
    ,file      = 1u<<9
};

template<typename Ar>
struct is_binary_archive: std::integral_constant<bool, Ar::type() == options::binary>
{};

template<typename Ar>
struct is_text_archive: std::integral_constant<bool, Ar::type() == options::text>
{};

template<typename Ar>
struct is_json_archive: std::integral_constant<bool, Ar::type() == options::json>
{};

template<typename Ar>
struct is_readable_archive: std::integral_constant<bool, Ar::is_readable()>
{};

template<typename Ar>
struct is_writable_archive: std::integral_constant<bool, Ar::is_writable()>
{};

/***************************************************************************/

namespace detail {

template<
     std::size_t F
    ,typename T
    ,bool Tok = std::is_integral<T>::value || std::is_enum<T>::value
>
struct can_be_processed_as_byte_array: std::integral_constant<bool,
    (is_any_of<T, char, signed char, unsigned char>::value) || // text/json
    ((F & yas::binary) && Tok && sizeof(T) == 1) ||
    ((F & yas::binary) && Tok && (!(F & yas::compacted) && (!__YAS_BSWAP_NEEDED(F))))
>
{};

} // ns detail

/***************************************************************************/

} // namespace yas

#endif // __yas__detail__type_traits__type_traits_hpp
