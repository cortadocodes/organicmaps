#include "../../testing/testing.hpp"
#include "../cellid.hpp"
#include "../../std/algorithm.hpp"
#include "../../std/string.hpp"

UNIT_TEST(CellID_Parent)
{
  TEST_EQUAL(m2::CellId<3>("1").Parent(), m2::CellId<3>(""), ());
  TEST_EQUAL(m2::CellId<4>("032").Parent(), m2::CellId<4>("03"), ());
}

UNIT_TEST(CellID_AncestorAtLevel)
{
  TEST_EQUAL(m2::CellId<3>("1").AncestorAtLevel(0), m2::CellId<3>(""), ());
  TEST_EQUAL(m2::CellId<4>("032").AncestorAtLevel(2), m2::CellId<4>("03"), ());
  TEST_EQUAL(m2::CellId<4>("032").AncestorAtLevel(1), m2::CellId<4>("0"), ());
  TEST_EQUAL(m2::CellId<4>("032").AncestorAtLevel(0), m2::CellId<4>(""), ());
}

UNIT_TEST(CellId_FromString)
{
  TEST_EQUAL(m2::CellId<3>(""), m2::CellId<3>::FromBitsAndLevel(0, 0), ());
  TEST_EQUAL(m2::CellId<4>("032"), m2::CellId<4>::FromBitsAndLevel(14, 3), ());
  TEST_EQUAL(m2::CellId<3>("03"), m2::CellId<3>::FromBitsAndLevel(3, 2), ());
}

UNIT_TEST(CellId_ToString)
{
  TEST_EQUAL(m2::CellId<3>("").ToString(), "", ());
  TEST_EQUAL(m2::CellId<4>("032").ToString(), "032", ());
  TEST_EQUAL(m2::CellId<3>("03").ToString(), "03", ());
}

UNIT_TEST(CellId_ToInt64)
{
  TEST_EQUAL(m2::CellId<3>("").ToInt64(), 1, ());
  TEST_EQUAL(m2::CellId<3>("0").ToInt64(), 2, ());
  TEST_EQUAL(m2::CellId<3>("1").ToInt64(), 7, ());
  TEST_EQUAL(m2::CellId<3>("2").ToInt64(), 12, ());
  TEST_EQUAL(m2::CellId<3>("3").ToInt64(), 17, ());
  TEST_EQUAL(m2::CellId<3>("00").ToInt64(), 3, ());
  TEST_EQUAL(m2::CellId<3>("01").ToInt64(), 4, ());
  TEST_EQUAL(m2::CellId<3>("03").ToInt64(), 6, ());
  TEST_EQUAL(m2::CellId<3>("10").ToInt64(), 8, ());
  TEST_EQUAL(m2::CellId<3>("20").ToInt64(), 13, ());
  TEST_EQUAL(m2::CellId<3>("23").ToInt64(), 16, ());
  TEST_EQUAL(m2::CellId<3>("30").ToInt64(), 18, ());
  TEST_EQUAL(m2::CellId<3>("31").ToInt64(), 19, ());
  TEST_EQUAL(m2::CellId<3>("33").ToInt64(), 21, ());
}

UNIT_TEST(CellId_FromInt64)
{
  TEST_EQUAL(m2::CellId<3>(""), m2::CellId<3>::FromInt64(1), ());
  TEST_EQUAL(m2::CellId<3>("0"), m2::CellId<3>::FromInt64(2), ());
  TEST_EQUAL(m2::CellId<3>("1"), m2::CellId<3>::FromInt64(7), ());
  TEST_EQUAL(m2::CellId<3>("2"), m2::CellId<3>::FromInt64(12), ());
  TEST_EQUAL(m2::CellId<3>("3"), m2::CellId<3>::FromInt64(17), ());
  TEST_EQUAL(m2::CellId<3>("00"), m2::CellId<3>::FromInt64(3), ());
  TEST_EQUAL(m2::CellId<3>("01"), m2::CellId<3>::FromInt64(4), ());
  TEST_EQUAL(m2::CellId<3>("03"), m2::CellId<3>::FromInt64(6), ());
  TEST_EQUAL(m2::CellId<3>("10"), m2::CellId<3>::FromInt64(8), ());
  TEST_EQUAL(m2::CellId<3>("20"), m2::CellId<3>::FromInt64(13), ());
  TEST_EQUAL(m2::CellId<3>("23"), m2::CellId<3>::FromInt64(16), ());
  TEST_EQUAL(m2::CellId<3>("30"), m2::CellId<3>::FromInt64(18), ());
  TEST_EQUAL(m2::CellId<3>("31"), m2::CellId<3>::FromInt64(19), ());
  TEST_EQUAL(m2::CellId<3>("33"), m2::CellId<3>::FromInt64(21), ());
}

UNIT_TEST(CellId_XY)
{
  TEST_EQUAL(m2::CellId<3>("").XY(), make_pair(4U, 4U), ());
  TEST_EQUAL(m2::CellId<3>("0").XY(), make_pair(2U, 2U), ());
  TEST_EQUAL(m2::CellId<3>("1").XY(), make_pair(6U, 2U), ());
  TEST_EQUAL(m2::CellId<3>("2").XY(), make_pair(2U, 6U), ());
  TEST_EQUAL(m2::CellId<3>("3").XY(), make_pair(6U, 6U), ());
  TEST_EQUAL(m2::CellId<3>("00").XY(), make_pair(1U, 1U), ());
  TEST_EQUAL(m2::CellId<3>("01").XY(), make_pair(3U, 1U), ());
  TEST_EQUAL(m2::CellId<3>("03").XY(), make_pair(3U, 3U), ());
  TEST_EQUAL(m2::CellId<3>("10").XY(), make_pair(5U, 1U), ());
  TEST_EQUAL(m2::CellId<3>("20").XY(), make_pair(1U, 5U), ());
  TEST_EQUAL(m2::CellId<3>("23").XY(), make_pair(3U, 7U), ());
  TEST_EQUAL(m2::CellId<3>("30").XY(), make_pair(5U, 5U), ());
  TEST_EQUAL(m2::CellId<3>("31").XY(), make_pair(7U, 5U), ());
  TEST_EQUAL(m2::CellId<3>("33").XY(), make_pair(7U, 7U), ());
  TEST_EQUAL(m2::CellId<3>("33").XY(), make_pair(7U, 7U), ());
}

UNIT_TEST(CellId_Radius)
{
  TEST_EQUAL(m2::CellId<3>("").Radius(), 4, ());
  TEST_EQUAL(m2::CellId<3>("1").Radius(), 2, ());
  TEST_EQUAL(m2::CellId<3>("00").Radius(), 1, ());
}

UNIT_TEST(CellId_FromXY)
{
  TEST_EQUAL((m2::CellId<3>::FromXY(0, 0)), (m2::CellId<3>("00")), ());
  TEST_EQUAL((m2::CellId<3>::FromXY(0, 0, 2)), (m2::CellId<3>("00")), ());
  TEST_EQUAL((m2::CellId<3>::FromXY(0, 0, 1)), (m2::CellId<3>("0")), ());
  TEST_EQUAL((m2::CellId<3>::FromXY(0, 0, 0)), (m2::CellId<3>("")), ());
  TEST_EQUAL((m2::CellId<3>::FromXY(5, 4, 0)), (m2::CellId<3>("")), ());
  TEST_EQUAL((m2::CellId<3>::FromXY(5, 0)), (m2::CellId<3>("10")), ());
  TEST_EQUAL((m2::CellId<3>::FromXY(5, 0, 1)), (m2::CellId<3>("1")), ());
  TEST_EQUAL((m2::CellId<3>::FromXY(5, 0, 1)), (m2::CellId<3>("1")), ());
  TEST_EQUAL((m2::CellId<3>::FromXY(7, 7, 2)), (m2::CellId<3>("33")), ());
  TEST_EQUAL((m2::CellId<3>::FromXY(7, 7, 1)), (m2::CellId<3>("3")), ());
  TEST_EQUAL((m2::CellId<3>::FromXY(7, 7, 0)), (m2::CellId<3>("")), ());
  TEST_EQUAL((m2::CellId<3>::FromXY(8, 8, 2)), (m2::CellId<3>("33")), ());
}

UNIT_TEST(CellId_FromXY_XY_Match)
{
  TEST_EQUAL((m2::CellId<9>::FromXY(48, 80).XY()), make_pair(49U, 81U), ());
  TEST_EQUAL((m2::CellId<9>::FromXY(192, 320).XY()), make_pair(193U, 321U), ());
  TEST_EQUAL((m2::CellId<11>::FromXY(768, 1280).XY()), make_pair(769U, 1281U), ());
  TEST_EQUAL((m2::CellId<21>::FromXY(786432, 1310720).XY()), make_pair(786433U, 1310721U), ());
}

UNIT_TEST(CellId_SubTreeSize)
{
  TEST_EQUAL(m2::CellId<3>("00").SubTreeSize(), 1, ());
  TEST_EQUAL(m2::CellId<3>("22").SubTreeSize(), 1, ());
  TEST_EQUAL(m2::CellId<3>("33").SubTreeSize(), 1, ());
  TEST_EQUAL(m2::CellId<3>("0").SubTreeSize(), 5, ());
  TEST_EQUAL(m2::CellId<3>("1").SubTreeSize(), 5, ());
  TEST_EQUAL(m2::CellId<3>("3").SubTreeSize(), 5, ());
  TEST_EQUAL(m2::CellId<3>("").SubTreeSize(), 21, ());
}

UNIT_TEST(CellID_LessQueueOrder)
{
  vector<string> v;
  v.push_back("0");
  v.push_back("1");
  v.push_back("00");
  v.push_back("00");
  v.push_back("02");
  v.push_back("002");
  v.push_back("101");
  vector<string> e = v;
  do
  {
    vector<m2::CellId<4> > tst, exp;
    for (size_t i = 0; i < v.size(); ++i)
    {
      tst.push_back(m2::CellId<4>(e[i]));
      exp.push_back(m2::CellId<4>(v[i]));
    }
    sort(tst.begin(), tst.end(), m2::CellId<4>::LessQueueOrder());
    TEST_EQUAL(tst, exp, ());
  } while (next_permutation(e.begin(), e.end()));
}

UNIT_TEST(CellID_LessStackOrder)
{
  vector<string> v;
  v.push_back("0");
  v.push_back("00");
  v.push_back("00");
  v.push_back("002");
  v.push_back("02");
  v.push_back("1");
  v.push_back("101");
  vector<string> e = v;
  do
  {
    vector<m2::CellId<4> > tst, exp;
    for (size_t i = 0; i < v.size(); ++i)
    {
      tst.push_back(m2::CellId<4>(e[i]));
      exp.push_back(m2::CellId<4>(v[i]));
    }
    sort(tst.begin(), tst.end(), m2::CellId<4>::LessStackOrder());
    TEST_EQUAL(tst, exp, ());
  } while (next_permutation(e.begin(), e.end()));
}

UNIT_TEST(CellID_IsStringValid)
{
  typedef m2::CellId<9> TId;
  TEST( TId::IsCellId("0123132"), () );
  TEST( !TId::IsCellId(""), () );
  TEST( !TId::IsCellId("-1332"), () );
  TEST( !TId::IsCellId("023."), () );
  TEST( !TId::IsCellId("121832"), () );
}
