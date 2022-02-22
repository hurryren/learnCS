#include<iostream>
using namespace std;
const float PI=3.14159;
const float FencePrice=35;
const float ConcretePrice=20;

class Circle{
	private:
		float radius;
	public:
		Circle(float r);
		float Circumference() const;
		float Area() const;
};

Circle::Circle(float r){
	radius = r;
}

/* 计算圆的周长 */
float Circle::Circumference() const {
	return 2 * PI * radius;
}

/* 计算圆的面积 */
float Circle::Area() const{
	return PI * radius * radius;
}


int main(int argc, char const *argv[]){
	float radius;
	float FenceCost, ConcreteCost;

	cout<<"Enter the radius of the poll: ";
	cin>>radius;

	Circle Pool(radius);
	Circle PoolRim(radius+3);

	FenceCost = PoolRim.Circumference() * FencePrice;
	cout << "fencing cost is : "<<FenceCost << endl;


	return 0;
}
