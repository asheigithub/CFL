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
		Rectangle 对象是按其位置（由它左上角的点 (x, y) 确定）以及宽度和高度定义的区域。 
		Rectangle 类的 x、y、width 和 height 属性相互独立；更改一个属性的值不会影响其他属性。
		*/
		struct Rectangle
		{
			Rectangle(float x = 0, float y  = 0, float width = 0, float height = 0)
				:x(x), y(y), width(width), height(height)
			{
				
			}

			float x;//矩形左上角的 x 坐标。
			float y;//矩形左上角的 y 坐标。
			float width;//矩形的宽度。
			float height;//矩形的高度。

			//比较两个Rectangle是否相同
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


			//矩形左上角的 x 坐标。
			float getLeft() const
			{
				return x;
			}
			//矩形左上角的 y 坐标。
			float getTop() const
			{
				return y;
			}

			//y 和 height 属性的和。
			float getBottom() const
			{
				return y + height;
			}
			//x 和 width 属性的和。
			float getRight() const
			{
				return x + width;
			}

			//由 right 和 bottom 属性的值确定的 Rectangle 对象的右下角的位置。
			Vector2 getBottomRight() const
			{
				return Vector2(getRight(), getBottom());
			}
			//由该点的 x 和 y 坐标确定的 Rectangle 对象左上角的位置。
			Vector2 getTopLeft() const
			{
				return Vector2(x, y);
			}

			//Rectangle 对象的大小，该对象表示为具有 width 和 height 属性的值的 Vector2 对象。
			Vector2 getSize() const
			{
				return Vector2(width, height);
			}


			//确定由此 Rectangle 对象定义的矩形区域内是否包含指定的点。
			bool contains(float x,float y ) const
			{
				return !(x < this->x || x > getRight() || y < this->y || y >getBottom());
			}
			//确定由此 Rectangle 对象定义的矩形区域内是否包含指定的点。
			bool contains(const Vector2& point) const
			{
				return contains(point.x, point.y);
			}

			//确定此 Rectangle 对象内是否包含由 rect 参数指定的 Rectangle 对象
			bool containsRect(const Rectangle& rect) const
			{
				return x <= rect.x && 
					getRight() >= rect.getRight() && 
					y <= rect.y && 
					getBottom() >= rect.getBottom();
			}

			//确定此 Rectangle 对象是否为空。
			bool isEmpty() const
			{
				return
					math::equalf(x, 0.0f) &&
					math::equalf(y, 0.0f) &&
					math::equalf(width, 0.0f) &&
					math::equalf(height, 0.0f)
					;
			}

			
			//确定在 toIntersect 参数中指定的对象是否与此 Rectangle 对象相交。
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

			//如果在 toIntersect 参数中指定的 Rectangle 对象与此 Rectangle 对象相交，则返回交集区域作为 Rectangle 对象。
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
			//通过填充两个矩形之间的水平和垂直空间，将这两个矩形组合在一起以创建一个新的 Rectangle 对象。
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
