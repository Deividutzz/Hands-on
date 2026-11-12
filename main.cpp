#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct StockProduct
{
    string name;
    long double productionPrice;
    long double quantity;
    int expirationDate;
};

struct OfferProduct
{
    string name;
    long double offerPrice;
    long double quantity;
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
    }

    void processOffers()
    {
        stable_sort(stock.begin(), stock.end(),
                    [](const StockProduct& left, const StockProduct& right)
                    {
                        return left.expirationDate < right.expirationDate;
                    });

        long double totalRevenue = 0;
        cout << setprecision(12);

        // Offers are processed in input order (FIFO).
        for (const Offer& offer : offers)
        {
            for (const OfferProduct& requested : offer.products)
            {
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
};

int main()
{
    MarketManager market;
    market.readInfo();
    market.processOffers();
    return 0;
}
