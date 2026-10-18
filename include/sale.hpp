#pragma once

#include <numeric>
#include <string>

struct SaleLine {
	std::string sku;
	int quantity;
	double priceAtSale = 0.0;
	int returnedQuantity = 0;
	SaleLine() = default;

	SaleLine(std::string itemSku, int qty, double price, int returnedQty = 0)
		:sky(std::move(itemSku)),
		quantity(qty),
		priceAtSale(price),
		returnedQuantity(returnedQty) {}
	// Remaining unnits that can still be returned
	int returnableQuantity() const {
		return quantity - returnedQuantity;
	}

	double lineTotal() const {
		return quantity * priceAtSale;
	}
};

struct Sale{
	std::string saleID;
	std::string timestamp;
	std::vector<SaleLine> lines;
	double total = 0;
	Sale() = default;
	
	Sale(std::string id, std::string time, std::vector<SaleLine> saleLines) 
		:	saleID(std::move(id)),
			timestamp(std::move(time)),
			lines(std::move(saleLines)) {
				calculateTotal();
			}
	void calculateTotal() {
		total = 0.0;
		for(const auto &line : lines) {
			total+= line.lineTotal();	
		}
	}
};