class MatrixBase:

    STR_MODEL = None
    STR_SERIAL_NUMBER = None
    STR_MANUFACTURE = None
    STR_FIRMWARE_VERSION = None
    STR_COMMUTATION = None

    MODEL = None
    SERIAL_NUMBER = None
    MANUFACTURE = None
    FIRMWARE_VERSION = None

    COMMUTATION: dict = None
    SWITCH_STATUS: dict = None

    # ------------------------------------------------------------------------------------------------------------------
    def auto_to_manual(self, input_, output_):
        return

    def turn_off(self):
        pack = list()
        for pcb in self.SWITCH_STATUS:
            for switch in self.SWITCH_STATUS[pcb]:
                pack.append((pcb, switch, 0))
        return pack

    # ------------------------------------------------------------------------------------------------------------------
    def check_switch_status(self, board, switch, ch):
        if ch == self.SWITCH_STATUS[board][switch]:
            return True
        else:
            return False

    def save_switch_status(self, board, switch, ch):
        self.SWITCH_STATUS[board][switch] = ch

    # ------------------------------------------------------------------------------------------------------------------

    def check_auto_switch_status(self, input_, output_):
        if output_ == self.COMMUTATION[input_]:
            return True
        else:
            return False

    def save_auto_switch_status(self, input_, output_):
        self.COMMUTATION[input_] = output_


class MatrixStatus(MatrixBase):

    STR_MODEL = 'MODEL'
    STR_SERIAL_NUMBER = 'SERIAL_NUMBER'
    STR_MANUFACTURE = 'MANUFACTURE'
    STR_FIRMWARE_VERSION = 'FIRMWARE_VERSION'
    STR_COMMUTATION = 'COMMUTATION'

    MODEL = 'FreqConvKL_1x2x6'
    SERIAL_NUMBER = 'RU123456789'
    MANUFACTURE = 'Tesart'
    FIRMWARE_VERSION = 'v1.0'

    BOARD = 8
    INPUTS = [1, 2, 3, 4, 5, 6]
    OUTPUTS = [1, 2]

    COMMUTATION = {input_: 0 for input_ in INPUTS}
    SWITCH_STATUS = {0: {switch: 0 for switch in INPUTS}}

    def auto_to_manual(self, input_, output_):
        return self.BOARD, input_, output_

