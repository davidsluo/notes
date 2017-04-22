/**
 * @author David Luo
 *         Represents an Item you can order.
 */
abstract class Item implements Product {

    private static final String MENU_PATTERN = "%1s%2s$%3$.2f\n";

    private int quantity;
    private double price;
    private String name;

    public Item(int quantity) {
        // will complain if this is false.
        assert quantity > 0;
        this.quantity = quantity;
    }

    /**
     * Helper method to print available choices.
     *
     * @param item  The name of the item on the menu.
     * @param price Corresponding price to the item.
     */
    protected static String getMenuLine(String item, double price) {
        StringBuilder filler = new StringBuilder();

        // Magic Number™
        int fillerAmount = 36 - String.format(MENU_PATTERN, item, "", price).length();

        for (int i = 0; i < fillerAmount; i++) {
            filler.append(".");
        }

        return String.format(MENU_PATTERN, item, filler.toString(), price);
    }

    public int getQuantity() {
        return quantity;
    }

    public void setQuantity(int quantity) {
        this.quantity = quantity;
    }

    public void addQuantity(int quantity) {
        this.quantity += quantity;
    }

    @Override
    public double calcTotal() {
        return price * quantity;
    }

    @Override
    public String getName() {
        return name;
    }

    void setName(String name) {
        this.name = name;
    }

    @Override
    public double getPrice() {
        return price;
    }

    void setPrice(double price) {
        this.price = price;
    }

}
