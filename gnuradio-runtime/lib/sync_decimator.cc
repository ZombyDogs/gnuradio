/* -*- c++ -*- */
/*
 * Copyright 2004,2013 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/sync_decimator.h>

namespace gr {

sync_decimator::sync_decimator(const std::string& name,
                               io_signature::sptr input_signature,
                               io_signature::sptr output_signature,
                               unsigned decimation)
    : sync_block(name, input_signature, output_signature)
{
    set_decimation(decimation);
}

void sync_decimator::forecast(int noutput_items, gr_vector_int& ninput_items_required)
{
    unsigned ninputs = ninput_items_required.size();
    for (unsigned i = 0; i < ninputs; i++)
        ninput_items_required[i] = fixed_rate_noutput_to_ninput(noutput_items);
}

int sync_decimator::fixed_rate_noutput_to_ninput(int noutput_items)
{
    return noutput_items * decimation() + history() - 1;
}

int sync_decimator::fixed_rate_ninput_to_noutput(int ninput_items)
{
    return std::max(0, ninput_items - (int)history() + 1) / decimation();
}

int sync_decimator::general_work(int noutput_items,
                                 gr_vector_int& ninput_items,
                                 gr_vector_const_void_star& input_items,
                                 gr_vector_void_star& output_items)
{
    int r = work(noutput_items, input_items, output_items);
    if (r > 0)
        consume_each(r * decimation());
    return r;
}

} /* namespace gr */
