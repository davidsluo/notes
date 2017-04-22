import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class Whatever extends JFrame implements ActionListener {
    private JLabel inputLabel;
    private JTextField inputField;
    private JComboBox<String> operationComboBox;
    private JTextField resultField;

    public Whatever() {
        super();
        inputLabel = new JLabel("Enter an Integer");
        inputField = new JTextField(5);
        operationComboBox = new JComboBox<>(new String[]{
                "Select operation",
                "Square",
                "Square root",
                "Factorial"
        });
        resultField = new JTextField(5);

        resultField.setEnabled(false);

        operationComboBox.addActionListener(this);
        inputField.addActionListener(this);

        this.add(inputLabel);
        this.add(inputField);
        this.add(operationComboBox);
        this.add(resultField);
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        if (e.getSource().equals(inputField) || e.getSource().equals(operationComboBox)) {
            if (inputField.getText().length() > 0 && !operationComboBox.getSelectedItem().equals("Select operation")) {
                processOperation();
            }
        }
    }

    private void processOperation() {
        int input;

        try {
            input = Integer.valueOf(inputField.getText());
        } catch (NumberFormatException e) {
            resultField.setText("");
            return;
        }

        switch ((String) operationComboBox.getSelectedItem()) {
            case "Square":
                resultField.setText(String.valueOf(Math.pow(input, 2)));
                break;
            case "Square root":
                resultField.setText(String.valueOf(Math.sqrt(input)));
                break;
            case "Factorial":
                try {
                    resultField.setText(String.valueOf(factorial(input)));
                } catch (NumberFormatException ex) {
                    resultField.setText("");
                }
                break;
            default:
                resultField.setText("");
                break;
        }

    }

    private int factorial(int input) {
        int output = 1;
        if (input == 0) {
            return 1;
        } else if (input < 0) {
            throw new NumberFormatException();
        } else {
            for (int i = 1; i <= input; i++) {
                output *= i;
            }
            return output;
        }
    }

    public static void main(String[] args) {
        Whatever whatever = new Whatever();
        whatever.setLayout(new FlowLayout());
        whatever.setVisible(true);
    }
}