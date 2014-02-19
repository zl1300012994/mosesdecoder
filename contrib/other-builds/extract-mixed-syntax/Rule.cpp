/*
 * Rule.cpp
 *
 *  Created on: 18 Feb 2014
 *      Author: s0565741
 */

#include <limits>
#include "Rule.h"
#include "Parameter.h"
#include "LatticeArc.h"
#include "ConsistentPhrases.h"

using namespace std;

Rule::Rule(const LatticeArc &arc)
:m_isValid(true)
,m_canExtend(true)
{
	m_arcs.push_back(&arc);
}

Rule::Rule(const Rule &prevRule, const LatticeArc &arc)
:m_arcs(prevRule.m_arcs)
,m_isValid(true)
,m_canExtend(true)
{
	m_arcs.push_back(&arc);
}

Rule::~Rule() {
	// TODO Auto-generated destructor stub
}

bool Rule::IsValid(const Parameter &params) const
{
  if (!m_isValid) {
	  return false;
  }

  return true;
}

bool Rule::CanExtend(const Parameter &params) const
{
  return true;
}

void Rule::Fillout(const ConsistentPhrases &consistentPhrases)
{
  // if last word is a non-term, check to see if it overlaps with any other non-terms
  if (m_arcs.back()->IsNonTerm()) {
	  const ConsistentRange *sourceRange = static_cast<const ConsistentRange *>(m_arcs.back());
	  const ConsistentRange &lastTargetRange = sourceRange->GetOtherRange();

	  for (size_t i = 0; i < m_arcs.size() - 1; ++i) {
		  const LatticeArc *arc = m_arcs[i];

		  if (arc->IsNonTerm()) {
			  const ConsistentRange *sourceRange = static_cast<const ConsistentRange *>(arc);
			  const ConsistentRange &targetRange = sourceRange->GetOtherRange();

			  if (lastTargetRange.Overlap(targetRange)) {
				  m_isValid = false;
				  m_canExtend = false;
				  return;
			  }
		  }
	  }
  }

  // find out if it's a consistent phrase
  int sourceStart = m_arcs.front()->GetStart();
  int sourceEnd = m_arcs.back()->GetEnd();

  int targetStart = numeric_limits<int>::max();
  int targetEnd = -1;

  for (size_t i = 0; i < m_arcs.size(); ++i) {
	  const LatticeArc &arc = *m_arcs[i];
	  if (arc.GetStart() < targetStart) {
		  targetStart = arc.GetStart();
	  }
	  if (arc.GetEnd() > targetEnd) {
		  targetEnd = arc.GetEnd();
	  }
  }

  m_consistentPhrase = consistentPhrases.Find(sourceStart, sourceEnd, targetStart, targetEnd);
  if (m_consistentPhrase != NULL) {
	  m_isValid = false;
	  return;
  }



}


Rule *Rule::Extend(const LatticeArc &arc) const
{
	Rule *ret = new Rule(*this, arc);

	return ret;
}
