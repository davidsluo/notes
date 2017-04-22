import java.util.ArrayList;
import java.util.List;

/**
 * @author David Luo
 *         Represents an order. Can have multiple Drinks, Pizzas, Appetizers, etc.
 */
public class Order {

    private static double TAX = 0.07;
    static final String OPTIONS_PROMPT =
            "Enter (1-3) to select an option or any other key to finish ordering.\n" +
                    "1) Order a pizza.\n" +
                    "2) Order an appetizer.\n" +
                    "3) Order a drink.";

    // All the items in this order
    private List<Item> items = new ArrayList<>();

    /**
     * Remove consolidate duplicate items in this order.
     */
    private void consolidateDuplicates() {

        List<Item> uniqueItems = new ArrayList<>();

        // For every item in the current list...
        for (Item item : items) {

            // If it's not in the uniqueItems list, add it...
            if (!uniqueItems.contains(item)) {
                uniqueItems.add(item);
            }
            // Otherwise, it's not unique, and ...
            else {
                // Search through the uniqueItems to find the unique item and...
                for (Item uItem : uniqueItems) {
                    if (item.equals(uItem)) {
                        // add the un-unique item's quantity to the unique item's quantity.
                        uItem.addQuantity(item.getQuantity());
                        break;
                    }
                }
            }
        }

        // The list is now unique.
        this.items = uniqueItems;
    }

    /**
     * Calculate the total cost of the items in the order, excluding tax.
     *
     * @return The total cost of the order.
     */
    public double calcTotal() {
        double total = 0;
        for (Item item : items) {
            total += item.calcTotal();
        }

        return total;
    }

    /**
     * Get a list of all the items in the order.
     *
     * @return The items in the order.
     */
    public List<Item> getItems() {
        return items;
    }

    /**
     * Add an item to the order.
     *
     * @param item The item to add to the order.
     */
    public void addItem(Item item) {
        items.add(item);
    }

    /**
     * Prints all available items for order.
     */
    static void printMenu() {
        System.out.println(Pizza.getMenu());
        System.out.println(Appetizer.getMenu());
        System.out.println(Drink.getMenu());
    }

    /**
     * Prints the invoice for this order.
     */
    public void printInvoice() {
        System.out.println(new Invoice(this).toString());
    }

    /**
     * Gets the number of <code>type</code> <code>Item</code>s in this <code>Order</code>.
     *
     * @param type The type of <code>Item</code> to count (i.e. Pizza, Appetizer, Drink)
     * @return The number of <code>type</code>s in this <code>Order</code>.
     */
    public int getCount(Class type) {
        int count = 0;
        // For each item in this order,
        //     if the item is an instance of type,
        //         increment count.
        for (Item item : items) {
            if (type.isInstance(item)) {
                count++;
            }
        }
        return count;
    }

    /**
     * @author David Luo
     *         Handles the generation of receipts/invoices
     */
    private static final class Invoice {

        private static final String INVOICE_LINE_PATTERN = "%1$-22s %2$9d $%3$7.2f\n";
        private static final String INVOICE_HEADER_PATTERN = "%1$-22s %2$9s %3$8s\n";
        private static final String SEPARATOR =
                genSeparatorLine(String.format(INVOICE_LINE_PATTERN, "", 0, 0.0).length());
        private static final String TOTAL_PATTERN = "%1$-32s $%2$7.2f\n";

        private final Order order;
        private final String invoiceString;

        /**
         * Constructor. Invoice only generated on initialization.
         *
         * @param order The order to generate an <code>Invoice</code> for.
         */
        Invoice(Order order) {
            order.consolidateDuplicates();
            this.order = order;
            this.invoiceString = genInvoice();
        }

        /**
         * Use this to get the invoice string.
         *
         * @return The invoice string.
         */
        @Override
        public String toString() {
            return invoiceString;
        }

        /**
         * Does the heavy lifting of generating the invoice.
         *
         * @return The invoice.
         */
        private String genInvoice() {
            // StringBuilder is better for long strings than just concatenating strings.
            StringBuilder builder = new StringBuilder();

            final String header = String.format(INVOICE_HEADER_PATTERN, "Item", "Qty", "Price");

//        builder.append("Invoice\n");
            builder.append(header);
            builder.append(SEPARATOR);

            // Add a line to the invoice for each item in the order.
            for (Item item : order.getItems()) {
                builder.append(String.format(INVOICE_LINE_PATTERN, item.getName(), item.getQuantity(), item.getPrice()));
            }

            builder.append(SEPARATOR);

            // Tax stuff
            builder.append(String.format(TOTAL_PATTERN, "Subtotal", order.calcTotal()));
            builder.append(String.format(TOTAL_PATTERN, "Tax", order.calcTotal() * Order.TAX));
            builder.append(String.format(TOTAL_PATTERN, "Total", order.calcTotal() * (1 + Order.TAX)));

            return builder.toString();
        }

        /**
         * Helper method to generate a separator line the same width as the other inputs.
         *
         * @param length Length of line to generate.
         * @return A line of '-' of length specified.
         */
        private static String genSeparatorLine(int length) {
            StringBuilder line = new StringBuilder();

            for (int i = 0; i < length - 1; i++)
                line.append('-');

            line.append('\n');

            return line.toString();
        }

    }

}