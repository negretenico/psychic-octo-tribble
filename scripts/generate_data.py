import struct

# Message types
MSG_NEW_ORDER = 1

def write_new_order(f, order_id, side, price, quantity):
    # Build payload
    payload = struct.pack(
        "<Q B Q I",
        order_id,     # uint64
        side,         # uint8
        price,        # uint64
        quantity      # uint32
    )

    # Record header
    msg_type = struct.pack("<B", MSG_NEW_ORDER)
    length   = struct.pack("<H", len(payload))

    # Write record
    f.write(msg_type)
    f.write(length)
    f.write(payload)

def main():
    with open("orders.bin", "wb") as f:
        write_new_order(f, order_id=1, side=1, price=1000, quantity=100)
        write_new_order(f, order_id=2, side=0, price=1010, quantity=50)
        write_new_order(f, order_id=3, side=1, price=995,  quantity=25)

    print("Wrote orders.bin")

if __name__ == "__main__":
    main()
