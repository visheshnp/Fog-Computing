#include "vivaldi.cpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <math.h>

using namespace std;
using ::testing::Return;
using ::testing::_;

class VivaldiMock1: public Vivaldi
{
public:
  MOCK_METHOD1(net_force_simple, Vivaldi::Coord(std::vector<Vivaldi::Sample> v));
 // MOCK_METHOD2(dist, double(Vivaldi::Coord a, Vivaldi::Coord b));
  VivaldiMock1(int lat,int lng,double e): Vivaldi(lat,lng,e){}

};

class VivaldiMock2: public Vivaldi
{
public:
  //MOCK_METHOD1(net_force_simple, Vivaldi::Coord(std::vector<Vivaldi::Sample> v));
  MOCK_METHOD2(dist, double(Vivaldi::Coord a, Vivaldi::Coord b));
  VivaldiMock2(int lat,int lng,double e): Vivaldi(lat,lng,e){}

};

TEST(VivaldiTest, Test1)
{   
    VivaldiMock1  mock(10,60,0.01);
    //Vivaldi::Coord c;
    Vivaldi::Coord c1;
    c1._lat = 10; 
    c1._lng = 30;
    Vivaldi::Coord c2;
    c2._lat = 3; 
    c2._lng = 4;
    Vivaldi::Coord f;
    f._lat = 10;
    f._lng = 10;
    Vivaldi::Sample other(c1,10,0.1);

    //s._c = c;
    //s._latency = 0;
    //s._error = 0;

    EXPECT_CALL(mock, net_force_simple(_))
        .Times(100).WillRepeatedly(Return(f));
        
   // EXPECT_EQ(0,0);  
    int count =0;
    while(count < 99)
    {
      mock.algorithm_simple(other);
      count ++;

    }
    
    EXPECT_GE(mock.algorithm_simple(other),60);

}


TEST(VivaldiTest, Test2)
{
    VivaldiMock2  mock(10,60,0.01);
    //Vivaldi::Coord c;
    Vivaldi::Coord c1;
    c1._lat = 10; 
    c1._lng = 30;
    Vivaldi::Coord c2;
    c2._lat = 3000; 
    c2._lng = 9000;
    Vivaldi::Coord f;
    f._lat = 0;
    f._lng = -20;
    Vivaldi::Sample other1(c1,10,0.1);
    Vivaldi::Sample other2(c2,20,0.1);

    //net_force_simple

   EXPECT_CALL(mock, dist(_,_))
      .WillOnce(Return(30));

    std::vector<Vivaldi::Sample> v;
    v.push_back(other1);
    //v.push_back(other2);

   EXPECT_EQ(mock.net_force_simple(v),f); 

}



int main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}