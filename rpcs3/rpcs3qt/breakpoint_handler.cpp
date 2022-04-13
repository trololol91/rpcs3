#include "breakpoint_handler.h"

extern bool ppu_breakpoint(u32 loc, bool is_adding);

bool breakpoint_handler::IsBreakOnBPM() const
{
	return m_break_on_bpm;
}

bool breakpoint_handler::HasBreakpoint(u32 loc, bs_t<breakpoint_types> type) const
{
	return m_breakpoints.contains(loc) && ((m_breakpoints.at(loc) & type) == type);
}

bool breakpoint_handler::AddBreakpoint(u32 loc, bs_t<breakpoint_types> type)
{
	if (type & breakpoint_types::bp_exec)
	{
		if (!ppu_breakpoint(loc, true))
		{
			return false;
		}
	}

	ensure(m_breakpoints.insert({loc, type}).second);
	return true;
}

bool breakpoint_handler::RemoveBreakpoint(u32 loc, bs_t<breakpoint_types> type)
{
	if (m_breakpoints.erase(loc) == 0)
	{
		return false;
	}

	if (type == breakpoint_types::bp_exec)
	{
		ensure(ppu_breakpoint(loc, false));
	}
	return true;
}
