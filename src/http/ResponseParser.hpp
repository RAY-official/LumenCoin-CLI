#pragma once

#include <tuple>
#include "types.hpp"

namespace http {

struct response;

class ResponseParser {
	enum state {
		http_version_h,
		http_version_t_1,
		http_version_t_2,
		http_version_p,
		http_version_slash,
		http_version_major_start,
		http_version_major,
		http_version_minor_start,
		http_version_minor,
		status_code_1,
		status_code_2,
		status_code_3,
		status_code_space,
		status_text,
		expecting_newline_1,
		header_line_start,
		header_lws,
		header_name,
		space_before_header_value,
		header_value,
		expecting_newline_2,
		expecting_newline_3,
		good,
		bad
	} state_;

public:
	ResponseParser();

	void reset();

	template<typename InputIterator>
	InputIterator parse(response &req, InputIterator begin, InputIterator end) {
		while (begin != end && state_ != good && state_ != bad)
			state_ = consume(req, *begin++);
		return begin;
	}
	bool is_good() const { return state_ == good; }
	bool is_bad() const { return state_ == bad; }

private:
	bool process_ready_header(response &req);
	Header lowcase;
	state consume(response &req, char input);
};

}  // namespace http
