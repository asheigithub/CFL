#ifndef CFL_RECTANGLE_H
#define CFL_RECTANGLE_H

#include "CFLGeom.h"
#include "../CFLString.h"
#include <sstream>

namespace cfl
{
	namespace geom
	{


		/*
		Rectangle �����ǰ���λ�ã��������Ͻǵĵ� (x, y) ȷ�����Լ���Ⱥ͸߶ȶ�������� 
		Rectangle ��� x��y��width �� height �����໥����������һ�����Ե�ֵ����Ӱ���������ԡ�
		*/
		struct Rectangle
		{
			Rectangle(float x = 0, float y  = 0, float width = 0, float height = 0)
				:x(x), y(y), width(width), height(height)
			{
				
			}

			float x;//�������Ͻǵ� x ���ꡣ
			float y;//�������Ͻǵ� y ���ꡣ
			float width;//���εĿ�ȡ�
			float height;//���εĸ߶ȡ�

			//�Ƚ�����Rectangle�Ƿ���ͬ
			bool equals(const Rectangle& rhs) const
			{
				return
					math::equalf(x, rhs.x) &&
					math::equalf(y, rhs.y) &&
					math::equalf(width, rhs.width) &&
					math::equalf(height, rhs.height);
			}

			bool operator==(const Rectangle& rhs) const
			{
				return equals(rhs);
			}
			bool operator!=(const Rectangle& rhs) const
			{
				return !equals(rhs);
			}


			//�������Ͻǵ� x ���ꡣ
			float getLeft() const
			{
				return x;
			}
			//�������Ͻǵ� y ���ꡣ
			float getTop() const
			{
				return y;
			}

			//y �� height ���Եĺ͡�
			float getBottom() const
			{
				return y + height;
			}
			//x �� width ���Եĺ͡�
			float getRight() const
			{
				return x + width;
			}

			//�� right �� bottom ���Ե�ֵȷ���� Rectangle ��������½ǵ�λ�á�
			Vector2 getBottomRight() const
			{
				return Vector2(getRight(), getBottom());
			}
			//�ɸõ�� x �� y ����ȷ���� Rectangle �������Ͻǵ�λ�á�
			Vector2 getTopLeft() const
			{
				return Vector2(x, y);
			}

			//Rectangle ����Ĵ�С���ö����ʾΪ���� width �� height ���Ե�ֵ�� Vector2 ����
			Vector2 getSize() const
			{
				return Vector2(width, height);
			}


			//ȷ���ɴ� Rectangle ������ľ����������Ƿ����ָ���ĵ㡣
			bool contains(float x,float y ) const
			{
				return !(x < this->x || x > getRight() || y < this->y || y >getBottom());
			}
			//ȷ���ɴ� Rectangle ������ľ����������Ƿ����ָ���ĵ㡣
			bool contains(const Vector2& point) const
			{
				return contains(point.x, point.y);
			}

			//ȷ���� Rectangle �������Ƿ������ rect ����ָ���� Rectangle ����
			bool containsRect(const Rectangle& rect) const
			{
				return x <= rect.x && 
					getRight() >= rect.getRight() && 
					y <= rect.y && 
					getBottom() >= rect.getBottom();
			}

			//ȷ���� Rectangle �����Ƿ�Ϊ�ա�
			bool isEmpty() const
			{
				return
					math::equalf(x, 0.0f) &&
					math::equalf(y, 0.0f) &&
					math::equalf(width, 0.0f) &&
					math::equalf(height, 0.0f)
					;
			}

			
			//ȷ���� toIntersect ������ָ���Ķ����Ƿ���� Rectangle �����ཻ��
			bool intersects(const Rectangle& toIntersect) const
			{
				return 
					!(
						getLeft() >= toIntersect.getRight() ||
						getTop() >= toIntersect.getBottom() ||
						getRight() <= toIntersect.getLeft() ||
						getBottom() <= toIntersect.getTop()
					)
					;

			}

			//����� toIntersect ������ָ���� Rectangle ������� Rectangle �����ཻ���򷵻ؽ���������Ϊ Rectangle ����
			Rectangle intersection(const Rectangle& toIntersect) const
			{
				if (intersects(toIntersect))
				{
					float left = math::maxf(x, toIntersect.x);
					float top = math::maxf(y, toIntersect.y);

					float bottom = math::minf(getBottom(), toIntersect.getBottom());
					float right = math::minf(getRight(), toIntersect.getRight());

					return Rectangle(left, top, right - left, bottom - top);
				}
				else
				{
					return Rectangle();
				}
			}
			//ͨ�������������֮���ˮƽ�ʹ�ֱ�ռ䣬�����������������һ���Դ���һ���µ� Rectangle ����
			Rectangle combine(const Rectangle& toUnion) const
			{
				float left = math::minf(x, toUnion.x);
				float top = math::minf(y, toUnion.y);

				float bottom = math::maxf(getBottom(), toUnion.getBottom());
				float right = math::maxf(getRight(), toUnion.getRight());

				return Rectangle(left, top, right - left, bottom - top);

			}


			CFLString toString() const
			{
				std::stringstream ss;
				ss << "(x=" << x << ",y=" << y << ",width=" <<width<< ",height=" <<height<< ")";
				return CFLString(ss.str().c_str());

			}


		};


	}
}


#endif // !CFL_RECTANGLE_H
