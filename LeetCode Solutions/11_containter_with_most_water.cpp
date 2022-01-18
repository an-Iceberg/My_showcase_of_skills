#include <vector>
#include <iostream>

// Returns the positive distance between two indices (provided they're not negative)
int distance(int &a, int &b)
{
    if (a > b)
    {
        return a - b;
    }
    else if (b > a)
    {
        return b - a;
    }
    else
    {
        return 0;
    }
}

int smallerPillar(int &pillarA, int &pillarB)
{
    if (pillarA < pillarB)
    {
        return pillarA;
    }
    else if (pillarB < pillarA)
    {
        return pillarB;
    }
    else
    {
        return pillarA;
    }
}

// TODO: rewrite this using pointers
// Calculates the highest possible area from a given vector of pillar heights
int maxArea(std::vector<int> &heights)
{
    int area = 0;
    int calculatedArea = 0;

    // ! Keep in mind that pillar and otherPillar are just indices not the actual height of the pillars (for the actual height use heights[pillar])

    // * Naive approach: for each pillar measure the area to all other pillars
    // * While it is inefficient in terms of time it does give us the correct resutls
    for (int pillar = 0; pillar < heights.size(); pillar++)
    {
        for (int otherPillar = 0; otherPillar < heights.size(); otherPillar++)
        {
            if (pillar == otherPillar)
            {
                continue;
            }

            calculatedArea = smallerPillar(heights[pillar], heights[otherPillar]) * distance(pillar, otherPillar);

            if (calculatedArea > area)
            {
                area = calculatedArea;
            }
        }
    }

    return area;
}

void test(int testID, std::vector<int> heights, int expected)
{
    int result = maxArea(heights);

    if (result != expected)
    {
        std::cout << testID << " failed expected:" << expected << " got:" << result << "\n";
    }
    else
    {
        std::cout << testID << " passed" << "\n";
    }
}

int main()
{
    // Tests from leetcode.com
    test(0, {1, 1}, 1);
    test(1, {4, 3, 2, 1, 4}, 16);
    test(2, {1, 2, 1}, 2);
    test(3, {1, 8, 6, 2, 5, 4, 8, 3, 7}, 49);

    // My own tests
    test(4, {1, 8, 2, 8, 3, 2, 5}, 25);
    test(5, {1, 3, 1, 1, 1, 1, 5, 1, 1, 1, 1, 4, 1}, 30);
    test(6, {1, 3, 1, 5, 1, 4, 1}, 12);
    return 0;
}
