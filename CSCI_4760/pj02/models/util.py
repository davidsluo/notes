def int_to_bytes(integer):
    """
    Helper method to convert 16-bit integers to a two-length byte.
    :param integer:
    :return:
    """
    return (integer).to_bytes(2, 'big')
