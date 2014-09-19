#include "osrm2feature_map.hpp"

#include "../defines.hpp"

#include "../base/assert.hpp"
#include "../base/logging.hpp"
#include "../base/math.hpp"

#include "../std/fstream.hpp"
#include "../std/sstream.hpp"


namespace routing
{

OsrmFtSegMapping::FtSeg::FtSeg(uint32_t fid, uint32_t ps, uint32_t pe)
  : m_fid(fid),
    m_pointStart(static_cast<uint16_t>(ps)),
    m_pointEnd(static_cast<uint16_t>(pe))
{
  CHECK_NOT_EQUAL(ps, pe, ());
  CHECK_EQUAL(m_pointStart, ps, ());
  CHECK_EQUAL(m_pointEnd, pe, ());
}

bool OsrmFtSegMapping::FtSeg::Merge(FtSeg const & other)
{
  if (other.m_fid != m_fid)
    return false;

  bool const dir = other.m_pointEnd > other.m_pointStart;
  if (dir != (m_pointEnd > m_pointStart))
    return false;

  auto const s1 = min(m_pointStart, m_pointEnd);
  auto const e1 = max(m_pointStart, m_pointEnd);
  auto const s2 = min(other.m_pointStart, other.m_pointEnd);
  auto const e2 = max(other.m_pointStart, other.m_pointEnd);

  if (my::IsIntersect(s1, e1, s2, e2))
  {
    m_pointStart = min(s1, s2);
    m_pointEnd = max(e1, e2);
    if (!dir)
      swap(m_pointStart, m_pointEnd);

    return true;
  }
  else
    return false;
}

bool OsrmFtSegMapping::FtSeg::IsIntersect(FtSeg const & other) const
{
  if (other.m_fid != m_fid)
    return false;

  auto const s1 = min(m_pointStart, m_pointEnd);
  auto const e1 = max(m_pointStart, m_pointEnd);
  auto const s2 = min(other.m_pointStart, other.m_pointEnd);
  auto const e2 = max(other.m_pointStart, other.m_pointEnd);

  return my::IsIntersect(s1, e1, s2, e2);
}

string DebugPrint(OsrmFtSegMapping::FtSeg const & seg)
{
  stringstream ss;
  ss << "{ fID = " << seg.m_fid <<
        "; pStart = " << seg.m_pointStart <<
        "; pEnd = " << seg.m_pointEnd << " }";
  return ss.str();
}


void OsrmFtSegMapping::Clear()
{
  m_offsets.clear();
  m_handle.Unmap();
}

void OsrmFtSegMapping::Load(FilesMappingContainer & cont)
{
  Clear();

  /// @todo To reduce memory pressure we can use regular Reader to load m_offsets.
  /// Also we can try to do mapping here.
  FilesMappingContainer::Handle h = cont.Map(ROUTING_NODEIND_TO_FTSEGIND_FILE_TAG);

  SegOffset const * p = h.GetData<SegOffset>();
  m_offsets.assign(p, p + h.GetDataCount<SegOffset>());

  h.Unmap();

  m_handle.Assign(cont.Map(ROUTING_FTSEG_FILE_TAG));
}

size_t OsrmFtSegMapping::GetSegmentsCount() const
{
  return m_handle.GetDataCount<FtSeg>();
}

pair<OsrmFtSegMapping::FtSeg const *, size_t> OsrmFtSegMapping::GetSegVector(OsrmNodeIdT nodeId) const
{
  pair<size_t, size_t> const r = GetSegmentsRange(nodeId);
  FtSeg const * p = GetSegments() + r.first;
  return make_pair(p, p->m_fid == -1 ? 0 : r.second);
}

void OsrmFtSegMapping::DumpSegmentsByFID(uint32_t fID) const
{
#ifdef DEBUG
  for (size_t i = 0; i < GetSegmentsCount(); ++i)
  {
    FtSeg const * p = GetSegments() + i;
    if (p->m_fid == fID)
      LOG(LDEBUG, (*p));
  }
#endif
}

void OsrmFtSegMapping::DumpSegmentByNode(uint32_t nodeId) const
{
#ifdef DEBUG
  auto const range = GetSegVector(nodeId);
  for (size_t i = 0; i < range.second; ++i)
    LOG(LDEBUG, (range.first[i]));
#endif
}

void OsrmFtSegMapping::GetOsrmNode(FtSeg const & seg, OsrmNodeIdT & forward, OsrmNodeIdT & reverse) const
{
  ASSERT_LESS(seg.m_pointStart, seg.m_pointEnd, ());

  OsrmNodeIdT const INVALID = -1;

  forward = reverse = INVALID;

  size_t const count = GetSegmentsCount();
  for (size_t i = 0; i < count; ++i)
  {
    FtSeg const & s = GetSegments()[i];
    if (s.m_fid != seg.m_fid)
      continue;

    if (s.m_pointStart <= s.m_pointEnd)
    {
      if (seg.m_pointStart >= s.m_pointStart && seg.m_pointEnd <= s.m_pointEnd)
      {
        ASSERT_EQUAL(forward, INVALID, ());
        forward = GetNodeId(i);
        if (reverse != INVALID)
          break;
      }
    }
    else
    {
      if (seg.m_pointStart >= s.m_pointEnd && seg.m_pointEnd <= s.m_pointStart)
      {
        ASSERT_EQUAL(reverse, INVALID, ());
        reverse = GetNodeId(i);
        if (forward != INVALID)
          break;
      }
    }
  }
}

pair<size_t, size_t> OsrmFtSegMapping::GetSegmentsRange(OsrmNodeIdT nodeId) const
{
  SegOffsetsT::const_iterator it = lower_bound(m_offsets.begin(), m_offsets.end(), SegOffset(nodeId, 0),
                                               [] (SegOffset const & o, SegOffset const & val)
                                               {
                                                  return (o.m_nodeId < val.m_nodeId);
                                               });

  size_t const index = distance(m_offsets.begin(), it);
  size_t const start = (index > 0  ? m_offsets[index - 1].m_offset + nodeId : nodeId);

  if (index < m_offsets.size() && m_offsets[index].m_nodeId == nodeId)
    return make_pair(start, m_offsets[index].m_offset + nodeId - start + 1);
  else
    return make_pair(start, 1);
}

OsrmNodeIdT OsrmFtSegMapping::GetNodeId(size_t segInd) const
{
  SegOffsetsT::const_iterator it = lower_bound(m_offsets.begin(), m_offsets.end(), SegOffset(segInd, 0),
                                               [] (SegOffset const & o, SegOffset const & val)
                                               {
                                                  return (o.m_nodeId + o.m_offset < val.m_nodeId);
                                               });

  size_t const index = distance(m_offsets.begin(), it);
  uint32_t const prevOffset = index > 0 ? m_offsets[index-1].m_offset : 0;

  if ((index < m_offsets.size()) &&
      (segInd >= prevOffset + m_offsets[index].m_nodeId) &&
      (segInd <= m_offsets[index].m_offset + m_offsets[index].m_nodeId))
  {
    return m_offsets[index].m_nodeId;
  }

  return (segInd - prevOffset);
}

OsrmFtSegMappingBuilder::OsrmFtSegMappingBuilder()
  : m_lastOffset(0)
{
}

void OsrmFtSegMappingBuilder::Append(OsrmNodeIdT osrmNodeId, FtSegVectorT const & data)
{
  if (data.empty())
    m_segments.push_back(FtSeg(-1, 0, 1));
  else
    m_segments.insert(m_segments.end(), data.begin(), data.end());

  if (data.size() > 1)
  {
    m_lastOffset += (data.size() - 1);

    uint32_t const off = static_cast<uint32_t>(m_lastOffset);
    CHECK_EQUAL(m_lastOffset, off, ());

    m_offsets.push_back(SegOffset(osrmNodeId, off));
  }
}

void OsrmFtSegMappingBuilder::Save(FilesContainerW & cont) const
{
  cont.GetWriter(ROUTING_FTSEG_FILE_TAG).Write(
        m_segments.data(), sizeof(FtSeg) * m_segments.size());

  cont.GetWriter(ROUTING_NODEIND_TO_FTSEGIND_FILE_TAG).Write(
        m_offsets.data(), sizeof(SegOffset) * m_offsets.size());
}

}
