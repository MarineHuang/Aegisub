/*
 * Copyright 2009-2017 Alibaba Cloud All rights reserved.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ALIBABANLS_COMMON_OUTCOME_H_
#define ALIBABANLS_COMMON_OUTCOME_H_

#include <utility>
//#include <iostream> // for debug

namespace AlibabaNlsCommon {
	template<typename E, typename R>
	class Outcome
	{
	public:
		Outcome() : success_(true), e_(), r_() { }

		explicit Outcome(const E &e) : e_(e), success_(false), r_() {}

		explicit Outcome(const R &r) : r_(r), success_(true), e_() {}

		explicit Outcome(R &&r) : r_(std::move(r)), success_(true), e_() {}

		Outcome(const Outcome &other) : 
			success_(other.success_),
			e_(other.e_),
			r_(other.r_) 
		{

		}

		Outcome(Outcome &&tmp) noexcept : 
			success_(tmp.success_),
			e_(std::move(tmp.e_)),
			r_(std::move(tmp.r_)) 
		{
			//std::cout << "Outcom move construct" << std::endl;
		}

		Outcome & operator=(const Outcome &other) 
		{
			if (this != &other) {
				success_ = other.success_;
				e_ = other.e_;
				r_ = other.r_;
			}
			return *this;
		}

		Outcome & operator=(Outcome &&tmp) noexcept
		{
			//std::cout << "Outcom move assign" << std::endl;
			if (this != &tmp) {
				success_ = tmp.success_;
				e_ = std::move(tmp.e_);
				r_ = std::move(tmp.r_);
			}
			return *this;
		}

		bool isSuccess()const {
            return success_;
        }

		const E& error() const { return e_; }
		const R& result() const { return r_; }

	private:
		bool success_;
		E e_;
		R r_;
	};
}
#endif // !ALIBABANLS_COMMON_OUTCOME_H_
