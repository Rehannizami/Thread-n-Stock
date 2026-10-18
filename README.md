`n`nThread & Stock — C++ Project Brief

C++ · multi-file console application · project specification

# Thread *&* StockDesign and build an inventory system for a small garment retailer. Track each sellable clothing variant, record the events that change stock, and give the shop owner dependable answers about what is on hand.

Intermediate projectConsole firstBuild it in milestonesNo finished solution included`n`n01 · The brief

## What you are buildingA menu-driven C++ application that helps a garment shop manage products, variant-level quantities, purchases from suppliers, sales, returns, and stock reports. Treat the shop as one store and save its data between runs.

**The key idea:** A “T-shirt” is a product style; “T-shirt / navy / size M” is a sellable stock item. Each size and color combination has its own SKU and quantity.

Shop owner needs

Find an item quickly, know whether it is in stock, record stock arriving or leaving, and spot items that need reordering.`n`nYour engineering challenge

Split responsibilities across headers and source files, connect the modules through clear interfaces, and keep inventory rules consistent.`n`n02 · Learning goals

## Skills you’ll practice### Organizing a C++ programUse declarations in ``.hpp`` files, definitions in matching ``.cpp`` files, and a small ``main.cpp`` to start the application.`n`n### Modeling a real domainSeparate a product style from its size/color variant; represent sales and stock adjustments as records with clear meaning.`n`n### Managing state safelyApply changes through service functions that enforce rules instead of letting menu code edit quantities anywhere.`n`n### Building dependable menusValidate input, handle missing records, format readable tables, and recover from invalid choices.`n`n### Saving and loadingRead and write structured data files, detect bad or missing files, and keep a usable application after restart.`n`n### Reasoning with testsTurn business rules into concrete scenarios, including edge cases such as selling the final unit or receiving a return.`n`n03 · Requirements

## What the first version must do### Product catalogue- Add a product style with name, category, brand, and optional description.
- Add variants with unique SKU, color, size, selling price, and reorder level.
- List and search products or variants by SKU, name, category, size, or color.
- Edit descriptive fields and prices; deactivate a variant instead of deleting history.`n`n### Stock operations- Receive stock for a selected SKU, recording quantity, date, and note/reference.
- Record a sale containing one or more SKU and quantity lines.
- Accept a return linked to a sale when possible; returned units increase stock.
- Record a manual adjustment with a reason, such as damaged or found stock.
- Prevent stock from becoming negative.`n`n### Shop reports- Current stock by SKU and total units across the shop.
- Low-stock list: variants at or below their reorder level.
- Inventory value estimate using quantity × unit cost, where cost is known.
- Sales summary over a date range and most-sold variants.
- Movement history for one SKU.`n`n### Application behavior- Provide a clear menu and return to it after each completed action.
- Give useful messages for invalid input and unavailable records.
- Load saved data at startup and save after successful changes or on exit.
- Confirm before potentially destructive actions such as deactivation.
- Use stable IDs/SKUs; never rely on a vector’s changing position as an identity.`n`n04 · File structure

## One project, several cooperating filesEach module owns one responsibility. A header tells other files what functions and types are available; the matching source file contains their implementation. Include the header where you need its declarations.

garment_inventory/
├── include/
│   ├── Product.hpp          // Product and Variant declarations
│   ├── Inventory.hpp        // Inventory operations interface
│   ├── Sale.hpp             // Sale and SaleLine declarations
│   ├── InventoryService.hpp // use-case/business-rule interface
│   ├── FileStorage.hpp      // persistence interface
│   └── ConsoleUI.hpp        // menu and input/output interface
├── src/
│   ├── Product.cpp          // product-related definitions (if needed)
│   ├── Inventory.cpp        // inventory lookup and basic operations
│   ├── Sale.cpp             // sale-related definitions
│   ├── InventoryService.cpp // receiving, selling, returns, adjustments
│   ├── FileStorage.cpp      // load/save implementation
│   ├── ConsoleUI.cpp        // prompts, menus, formatted display
│   └── main.cpp             // construct objects, load data, run UI
├── data/
│   ├── products.csv         // persisted product and variant records
│   ├── movements.csv        // stock movement history
│   └── sales.csv            // sale headers and lines (or a documented format)
└── README.md

**Dependency direction:** ``main.cpp`` wires the pieces together. The UI asks the service to perform an action; the service checks rules and updates inventory; storage loads and saves records. Keep business rules out of the menu prompts.

`n| | File pair | | Public interface belongs in | | Definitions belong in | | Responsibility | 

| Product | | ``Product.hpp`` | | ``Product.cpp`` (optional) | | Product style and sellable variant data | 

| Inventory | | ``Inventory.hpp`` | | ``Inventory.cpp`` | | Keep records and find a variant by SKU | 

| Service | | ``InventoryService.hpp`` | | ``InventoryService.cpp`` | | Validate and perform business operations | 

| Storage | | ``FileStorage.hpp`` | | ``FileStorage.cpp`` | | Convert records to/from persistent files | 

| UI | | ``ConsoleUI.hpp`` | | ``ConsoleUI.cpp`` | | Menus, prompts, output formatting | 

| Program entry | | Usually none | | ``main.cpp`` | | Build objects, load data, launch the menu loop | 

`n

Use include guards or ``#pragma once``. Include standard-library headers in the file that uses them. Compile every ``.cpp`` file and link them into one executable; do not include a ``.cpp`` file from another.`n`n05 · Data model

## Suggested records and relationshipsChoose types that match the information. Start simple and document decisions such as whether sizes are fixed labels or free text.

`n| | Record | | Suggested fields | | Purpose | 

| Product | | productId, name, brand, category, description, active | | Shared details for a style, such as “Classic Crew T-shirt” | 

| Variant | | sku, productId, size, color, unitPrice, unitCost, quantity, reorderLevel, active | | One sellable combination with its own stock count | 

| Sale | | saleId, timestamp, lines, total | | One checkout transaction | 

| SaleLine | | sku, quantity, priceAtSale | | What was sold and the price at that time | 

| StockMovement | | movementId, sku, type, quantity, timestamp, reference, note | | Audit trail for received, sold, returned, or adjusted stock | 

`n

**Design choice to explain:** Current quantity can be stored on each variant for fast lookup, while every change is also recorded as a movement. Your rules should keep the quantity and movement history in agreement.`n`n06 · Shop workflows

## Think in complete journeys1

**Opening the shop:** application starts → storage loads records → invalid or missing data is reported clearly → menu appears.`n`n2

**Receiving stock:** staff selects SKU → enters positive quantity and reference → service verifies active variant → quantity rises → movement is recorded → data is saved.`n`n3

**Making a sale:** staff adds one or more SKU lines → system checks each SKU and available quantity → total is calculated → all lines are committed together → sale and stock movements are saved.`n`n4

**Handling a return:** staff finds original sale → selects a line and return quantity → system ensures it has not already been returned → inventory rises and return movement is recorded.`n`n5

**Reordering:** owner opens low-stock report → sees SKU, variant details, current quantity, and reorder level → uses the list to plan supplier order.`n`nFor a multi-line sale, decide how to avoid a half-completed transaction: validate every line first, then apply all quantity changes only if every line is valid.`n`n07 · Rules & validation

## Guardrails for trustworthy stock### Identity and catalogue- SKU is required and unique, ignoring accidental surrounding spaces.
- Every variant refers to an existing product.
- Price, cost, and reorder level cannot be negative.
- Names and category cannot be blank.
- Deactivated variants remain visible in old sales and movements.`n`n### Quantities and transactions- Movement quantities must be positive; movement type determines whether stock rises or falls.
- A sale cannot exceed available stock and cannot contain zero/negative quantities.
- Reject a transaction without changing any stock if any line is invalid.
- Return quantity cannot exceed the quantity sold minus earlier returns.
- Use the sale-time price for historical totals, even if the current price changes.`n`n### Input and persistence- Handle non-numeric input without leaving the input stream stuck.
- Check file-open and parse errors; tell the user which data could not be loaded.
- Do not silently overwrite corrupted or unrecognized records.
- Use one documented date/time format and one documented delimiter/escaping rule.
- Keep a backup or write a temporary file before replacing saved data (advanced reliability).`n`n### Money and reports- Choose a currency and display it consistently.
- For a stronger design, represent money as integer minor units (for example paise/cents), not floating-point.
- Define low stock as quantity less than or equal to reorder level.
- Define whether inactive variants appear in current-stock reports.
- Make date-range boundaries explicit and consistent.`n`n08 · Build milestones

## Grow it one working slice at a time1

**Project skeleton.** Create the folders and files. Add a tiny program that includes your own header and builds. Confirm all source files are part of the build.`n`n2

**Catalogue in memory.** Define Product and Variant. Add a few sample records; list them and search by SKU. No file saving yet.`n`n3

**Inventory service.** Implement receive, adjustment, and sale operations. Centralize validation and stock changes in the service layer.`n`n4

**Console menus.** Add catalogue, stock, sales, and report menus. Keep prompts and table formatting in the UI module.`n`n5

**Persistence.** Save and load records using a documented format. Test that closing and reopening preserves the same stock and history.`n`n6

**History and returns.** Add movement records, sale history, and return limits linked to original sale lines.`n`n7

**Polish.** Improve error messages, confirm risky actions, add sample data, and write a README with build/run instructions and design decisions.`n`n09 · Test scenarios

## Prove the rules with examples`n| | Scenario | | Action | | Expected result | 

| Duplicate SKU | | Add a variant with an existing SKU | | Rejected; existing record remains unchanged | 

| Receive goods | | Receive 12 units for a valid SKU | | Quantity rises by 12; one receive movement appears | 

| Oversell | | Sell more units than available | | Sale rejected; stock and sale history do not change | 

| Multi-line failure | | First line is valid, second line requests too much | | Entire sale rejected; first line is not deducted | 

| Exact stock sale | | Sell exactly the available quantity | | Sale succeeds; quantity becomes zero | 

| Low-stock boundary | | Quantity equals reorder level | | SKU appears in low-stock report | 

| Return over limit | | Return more than sold minus prior returns | | Rejected; no stock increase | 

| Price changed later | | Change current price after a sale | | Old sale report keeps the original sale price | 

| Restart | | Save, exit, reopen | | Products, quantities, sales, and movements load correctly | 

| Bad file/input | | Enter text where a number is expected or provide malformed data | | Clear error; program remains usable; no silent data loss | 

`n`n`n10 · Done means

## Acceptance checklist- The application compiles from the documented build instructions with all required ``.cpp`` files.
- Each module has a clear responsibility and uses headers to expose its interface.
- A user can add, find, list, update, and deactivate product variants.
- Receiving, sale, return, and adjustment operations enforce stock rules and create a history record.
- Reports show correct quantities, low-stock items, and sales totals for a chosen period.
- Invalid menu input and invalid business operations produce clear feedback.
- Saved data survives closing and reopening the program.
- The README explains setup, build/run steps, data format, limitations, and design decisions.
- All scenarios in the test table have been tried and their outcomes match expectations.`n`n11 · Stretch goals

## Only after the core is reliableStorage### SQLite databaseReplace flat files with tables and transactions while preserving the service interface.`n`nOperations### Supplier & purchase ordersTrack suppliers, draft orders, expected deliveries, and receiving against an order.`n`nQuality### Automated unit testsTest the service and validation without entering the console menus.`n`nInsights### More useful reportsSales by category, gross margin estimate, dead stock, and reorder suggestions.`n`nSafety### Undo and audit toolsCorrect mistakes with reversing movements instead of erasing history.`n`nInterface### CSV exportExport stock and sales reports for a spreadsheet, with proper CSV escaping.`n`n**Your implementation path:** Get one small slice working before expanding it. Start with two products and a handful of variants, and make every stock change explainable through a recorded operation.`n`n**Thread & Stock** · A build-it-yourself C++ project brief

Specification only. The design is intentionally open enough for you to make and document your own implementation choices.`n`n

