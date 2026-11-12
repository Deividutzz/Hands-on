#include <algorithm>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

struct StockProduct
{
    string name;
    long double productionPrice;
    long double quantity;
    int expirationDate = -1;
};

struct OfferProduct
{
    string name;
    long double offerPrice;
    long double quantity;
};

struct Ingredient
{
    string name;
    long double quantity;
};

struct Meal
{
    string name;
    vector<Ingredient> ingredients;
};

struct Offer
{
    int date;
    vector<OfferProduct> products;
};

class MarketManager
{
public:
    void readInfo()
    {
        int stockCount;
        cin >> stockCount;
        stock.reserve(stockCount);

        for (int i = 0; i < stockCount; ++i)
        {
            StockProduct product;
            cin >> product.name >> product.productionPrice
                >> product.quantity >> product.expirationDate;
            stock.push_back(product);
        }
        int offerCount;
        cin >> offerCount;
        offers.reserve(offerCount);

        for (int i = 0; i < offerCount; ++i)
        {
            Offer offer;
            int productCount;
            cin >> offer.date >> productCount;
            offer.products.reserve(productCount);

            for (int j = 0; j < productCount; ++j)
            {
                OfferProduct product;
                cin >> product.name >> product.offerPrice >> product.quantity;
                offer.products.push_back(product);
            }

            offers.push_back(offer);
        }

        int mealCount;
        cin >> mealCount;
        meals.reserve(mealCount);

        for (int i = 0; i < mealCount; ++i) {
            Meal meal;
            int ingredientCount;
            cin >> meal.name >> ingredientCount;
            meal.ingredients.reserve(ingredientCount);

            for (int j = 0; j < ingredientCount; ++j) {
                Ingredient ingredient;
                cin >> ingredient.name >> ingredient.quantity;
                meal.ingredients.push_back(ingredient);
            }
            meals.push_back(meal);
        }
    }

    void processOffers()
    {
        long double totalRevenue = 0;
        cout << setprecision(12);

        // Offers are processed in input order (FIFO).
        for (const Offer& offer : offers)
        {
            for (const OfferProduct& requested : offer.products)
            {
                if (const Meal* meal = findMeal(requested.name))
                {
                    map<string, long double> ingredientsPerMeal;
                    long double productionPrice = 0;
                    vector<string> mealPath{meal->name};
                    bool validRecipe = true;

                    for (const Ingredient& ingredient : meal->ingredients)
                    {
                        if (!expandIngredient(ingredient.name,
                                              ingredient.quantity,
                                              ingredientsPerMeal,
                                              productionPrice,
                                              mealPath))
                        {
                            validRecipe = false;
                            break;
                        }
                    }

                    if (!validRecipe || requested.offerPrice < productionPrice)
                        continue;

                    long double mealQuantity = requested.quantity;
                    for (const auto& [ingredientName, quantityPerMeal] :
                         ingredientsPerMeal)
                    {
                        long double availableQuantity = 0;
                        for (const StockProduct& available : stock)
                        {
                            if (available.name == ingredientName &&
                                available.expirationDate > offer.date)
                            {
                                availableQuantity += available.quantity;
                            }
                        }

                        mealQuantity = min(mealQuantity,
                                           availableQuantity / quantityPerMeal);
                    }

                    if (mealQuantity <= 0)
                        continue;

                    for (const auto& [ingredientName, quantityPerMeal] :
                         ingredientsPerMeal)
                    {
                        long double remaining =
                            mealQuantity * quantityPerMeal;
                        for (StockProduct& available : stock)
                        {
                            if (available.name != ingredientName ||
                                available.expirationDate <= offer.date)
                            {
                                continue;
                            }

                            const long double quantityTaken =
                                min(remaining, available.quantity);
                            available.quantity -= quantityTaken;
                            remaining -= quantityTaken;
                            if (remaining <= 1e-12L)
                                break;
                        }
                    }

                    cout << requested.name << ' ' << mealQuantity << '\n';
                    totalRevenue += mealQuantity * requested.offerPrice;
                    continue;
                }

                long double remaining = requested.quantity;
                long double sold = 0;

                for (StockProduct& available : stock)
                {
                    if (available.quantity <= 0 ||
                        available.name != requested.name ||
                        available.expirationDate <= offer.date ||
                        requested.offerPrice < available.productionPrice)
                    {
                        continue;
                    }

                    const long double quantityTaken =
                        min(remaining, available.quantity);
                    available.quantity -= quantityTaken;
                    remaining -= quantityTaken;
                    sold += quantityTaken;

                    if (remaining <= 1e-12L)
                    {
                        remaining = 0;
                        break;
                    }
                }

                if (sold > 0)
                {
                    cout << requested.name << ' ' << sold << '\n';
                    totalRevenue += sold * requested.offerPrice;
                }
            }

            // Expired products cannot be sold on later offer dates.
            for (StockProduct& available : stock)
            {
                if (available.expirationDate <= offer.date)
                    available.quantity = 0;
            }
        }

        cout << totalRevenue << '\n';
    }

private:
    vector<StockProduct> stock;
    vector<Offer> offers;
    vector<Meal> meals;

    const Meal* findMeal(const string& name) const
    {
        for (const Meal& meal : meals)
        {
            if (meal.name == name)
                return &meal;
        }
        return nullptr;
    }

    bool expandIngredient(const string& name,
                          long double quantity,
                          map<string, long double>& rawIngredients,
                          long double& productionPrice,
                          vector<string>& mealPath) const
    {
        if (quantity == 0)
            return true;

        if (const Meal* meal = findMeal(name))
        {
            if (find(mealPath.begin(), mealPath.end(), meal->name) !=
                mealPath.end())
            {
                return false;
            }

            mealPath.push_back(meal->name);
            for (const Ingredient& ingredient : meal->ingredients)
            {
                if (!expandIngredient(ingredient.name,
                                      quantity * ingredient.quantity,
                                      rawIngredients,
                                      productionPrice,
                                      mealPath))
                {
                    mealPath.pop_back();
                    return false;
                }
            }
            mealPath.pop_back();
            return true;
        }

        for (const StockProduct& product : stock)
        {
            if (product.name == name)
            {
                rawIngredients[name] += quantity;
                productionPrice += quantity * product.productionPrice;
                return true;
            }
        }
        return false;
    }
};

int main()
{
    MarketManager market;
    market.readInfo();
    market.processOffers();
    return 0;
}
