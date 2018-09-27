// Inspired by: http://www.codeproject.com/Articles/210979/Fast-optimizing-rectangle-packing-algorithm-for-bu (i think)

#include <Sauce/math.h>

BEGIN_SAUCE_NAMESPACE

bool heightSort(RectanglePacker::Entry i, RectanglePacker::Entry j)
{
	return i.getHeight() > j.getHeight();
}

const RectanglePacker::Result RectanglePacker::pack()
{
	// No point in packing 0 rectangles
	if(m_rectangles.size() == 0)
	{
		return Result();
	}

	// Sort rectangles by height
	sort(m_rectangles.begin(), m_rectangles.end(), heightSort);

	// Find total area and max height
	uint totalArea = 0;
	uint rightMost = 0;
	uint maxWidth = 0, maxHeight = m_rectangles[0].getHeight();
	for(vector<Entry>::iterator itr = m_rectangles.begin(); itr != m_rectangles.end(); itr++)
	{
		Entry &rect = *itr;
		totalArea += rect.getHeight() * rect.getHeight();
		if(maxWidth < rect.getWidth())
		{
			maxWidth = rect.getWidth();
		}
	}

	// Setup loop vars
	uint canvasWidth = m_maxWidth, canvasHeight = maxHeight;
	vector<Rect<int>> cells;
	cells.push_back(Rect<int>(0, 0, canvasWidth, canvasHeight));
	Result bestResult;
	Result result;

	// Find optimal canvas rect by testing
	int idx = 0;
	while(canvasWidth >= maxWidth)
	{
		// Get rectangle
		Entry &rect = m_rectangles[idx++];

		// Find best cell
		int bestCellIdx = -1;
		for(uint i = 0; i < cells.size(); i++)
		{
			Rect<int> *cell = &cells[i];
			if((uint) cell->getWidth() >= rect.getWidth() && (uint) cell->getHeight() >= rect.getHeight())
			{
				if(bestCellIdx < 0 || cell->getArea() < cells[bestCellIdx].getArea())
					bestCellIdx = i;
			}
		}

		if(bestCellIdx < 0)
		{
			// If no cell was found, add more to the height and retry
			cells.clear();
			cells.push_back(Rect<int>(0, 0, canvasWidth, ++canvasHeight));
			result.clear();
			idx = rightMost = 0;
		}
		else
		{
			// Store cell and remove
			Rect<int> cell = cells[bestCellIdx];
			cells.erase(cells.begin() + bestCellIdx);

			// Place rectangle into results
			rect.position = cell.position;
			result.rectangles[rect.key] = rect;
			if(rect.getX() + rect.getWidth() > rightMost)
			{
				rightMost = rect.getX() + rect.getWidth();
			}

			// Split the cell into 2 smaller cells
			Rect<int> c1(cell.getX() + rect.getWidth(), cell.getY(), cell.getWidth() - rect.getWidth(), rect.getHeight());
			Rect<int> c2(cell.getX(), cell.getY() + rect.getHeight(), cell.getWidth(), cell.getHeight() - rect.getHeight());

			if(c1.getArea() > 0) cells.push_back(c1);
			if(c2.getArea() > 0) cells.push_back(c2);

			// Check for end
			if(idx == (int) m_rectangles.size())
			{
				// Success, check for the smalest area
				result.area = rightMost*canvasHeight;
				if(!bestResult.valid || result.area < bestResult.area)
				{
					// Set start width
					if(!bestResult.valid)
						canvasWidth = rightMost;

					// Store canvas dimensions and the result as best
					result.valid = true;
					result.canvas.set(rightMost, canvasHeight);
					bestResult = result;
				}

				// Retry with different dimensions
				cells.clear();
				cells.push_back(Rect<int>(0, 0, --canvasWidth, canvasHeight));
				result.clear();
				idx = rightMost = 0;
			}
		}
	}
	return bestResult;
}

void RectanglePacker::addRectangle(const string &key, const uint width, const uint height, void *data)
{
	m_rectangles.push_back(Entry(key, width, height, data));
}

void RectanglePacker::clear()
{
	m_rectangles.clear();
}

END_SAUCE_NAMESPACE
 