/**
 * @author David Luo
 *         Represents any product sold by the pizza shop (e.g. food or services; food is represented by Item class)
 */
public interface Product {
    /**
     * @return Price of Quantity count of Products
     */
    double calcTotal();

    /**
     * @return The formalized name of the <code>Product</code>.
     */
    String getName();

    /**
     * @return The cost of one unit of the <code>Product</code>.
     */
    double getPrice();
}