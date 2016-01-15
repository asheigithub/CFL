#ifndef VLITABLE_H
#define VLITABLE_H

//变长整数编码表

namespace cfl
{

	namespace vli
	{

		struct VLI
		{
			unsigned char bits;//占用位数
			short value;	   //编码值
		};

		struct VLITableRow
		{
			int rowno;
			short low1;
			short high1;
			short low2;
			short high2;
		};

		struct VLItable
		{
			VLItable()
			{
				auto nums = 1;

				for (int i = 0; i < 16; i++)
				{
					rows[i].rowno = i;
					if (i == 0)
					{
						rows[0].low1 = 0;
						rows[0].high1 = 0;
						rows[0].low2 = 0;
						rows[0].high2 = 0;
					}
					else
					{
						auto last = &rows[i - 1];
						auto row = &rows[i];

						row->high1 = last->low1 - 1;
						row->low1 = last->low1 - nums;
						row->low2 = last->high2 + 1;
						row->high2 = last->high2 + nums;
						nums = nums * 2;
					}
				}
			}

			VLITableRow rows[15];

		};
		
		static VLItable table;

		
		short getValue(int row, int idx)
		{
			if (row == 0)
			{
				return 0;
			}

			auto r = &table.rows[row];
			if (idx <= r->high1 - r->low1)
			{
				return r->low1 + idx;
			}
			else
			{
				return r->low2 + idx - 1 - (r->high1 - r->low1);
			}
		}

	}
}



#endif // !VLITABLE_H
