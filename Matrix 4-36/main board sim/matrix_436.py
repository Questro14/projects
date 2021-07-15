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

    MODEL = 'SwitchMatrix_4x36'
    SERIAL_NUMBER = 'RU123456789'
    MANUFACTURE = 'Tesart'
    FIRMWARE_VERSION = 'v1.0'

    COMMUTATION = {
        # input: output
        1: 0,
        2: 0,
        3: 0,
        4: 0,
    }

    INPUT_CONV_RANGE = [1, 2]
    INPUT_RANGE = [3, 4]
    OUTPUT_CONV_RANGE = range(0, 38)
    OUTPUT_RANGE = range(0, 37)
    PCB_RANGE = [1, 2, 3, 4, 5, 6, 7]
    SWITCH_RANGE = range(1, 11)
    SWITCH_OUTPUT_RANGE = range(0, 7)

    __PCB = 'PCB'
    __SWITCH_OUT = 'SWITCH_OUT'
    __SWITCH_IN = 'SWITCH_IN'
    __CONV1 = 'CONV1'
    __CONV2 = 'CONV2'
    __CONV_SWITCH_ON = "Turn On"
    __CONV_SWITCH_OFF = "Turn Off"
    __BS_SW_IN = [1, 2, 7, 8]
    __BS_SW_OUT = [3, 4, 5, 6, 9, 10]
    __BS_OUTS = {
        # PCB: outputs according to output_switch order
        PCB_RANGE[1]: [13, 14, 15, 16, 17, 18],
        PCB_RANGE[2]: [30, 29, 28, 27, 26, 25],
        PCB_RANGE[3]: [36, 35, 34, 33, 32, 31],
        PCB_RANGE[4]: [19, 20, 21, 22, 23, 24],
        PCB_RANGE[5]: [7, 8, 9, 10, 11, 12],
        PCB_RANGE[6]: [1, 2, 3, 4, 5, 6]
    }
    # For board 2-7
    __SW_IN_TO_SW_OUT = {
        # PCB input_switch: PCB input_switch outputs according to output_switch order
        __BS_SW_IN[0]: [4, 5, 6, 1, 2, 3],
        __BS_SW_IN[1]: [5, 4, 3, 2, 1, 6],
        __BS_SW_IN[2]: [3, 4, 5, 6, 1, 2],
        __BS_SW_IN[3]: [6, 5, 4, 3, 2, 1]
    }
    # __SW_OUT_TO_SW_IN = {
    #     # PCB output_switch: PCB output_switch inputs according to input_switch order
    #     __BS_SW_OUT[0]: [3, 6, 2, 5],
    #     __BS_SW_OUT[1]: [3, 5, 2, 6],
    #     __BS_SW_OUT[2]: [3, 5, 2, 6],
    #     __BS_SW_OUT[3]: [3, 5, 2, 6],
    #     __BS_SW_OUT[4]: [3, 5, 2, 6],
    #     __BS_SW_OUT[5]: [2, 5, 3, 6]
    # }

    # For board 1
    __SW_OUT_TO_SW_IN = {
        # PCB output_switch: PCB output_switch inputs according to input_switch order
        __BS_SW_OUT[0]: [2, 4, 1, 3],
        __BS_SW_OUT[1]: [2, 3, 1, 4],
        __BS_SW_OUT[2]: [2, 3, 1, 4],
        __BS_SW_OUT[3]: [2, 3, 1, 4],
        __BS_SW_OUT[4]: [2, 3, 1, 4],
        __BS_SW_OUT[5]: [1, 3, 2, 4]
    }
    # INPUTS_COMMUTATION = {
    #     1: {__SWITCH_OUT: 5, __SWITCH_IN: 2, __CONV1: {__SWITCH_OUT: 12, __CONV_SWITCH_ON: 2, __CONV_SWITCH_OFF: 1}},
    #     2: {__SWITCH_OUT: 1, __SWITCH_IN: 1, __CONV2: {__SWITCH_OUT: 11, __CONV_SWITCH_ON: 1, __CONV_SWITCH_OFF: 2}},
    #     3: {__SWITCH_OUT: 6, __SWITCH_IN: 3},
    #     4: {__SWITCH_OUT: 2, __SWITCH_IN: 4}
    # }
    INPUTS_COMMUTATION = {
        1: {__SWITCH_OUT: 1, __SWITCH_IN: 2, __CONV1: {__SWITCH_OUT: 10, __CONV_SWITCH_ON: 2, __CONV_SWITCH_OFF: 1}},
        2: {__SWITCH_OUT: 2, __SWITCH_IN: 1, __CONV2: {__SWITCH_OUT: 10, __CONV_SWITCH_ON: 3, __CONV_SWITCH_OFF: 4}},
        3: {__SWITCH_OUT: 7, __SWITCH_IN: 7},
        4: {__SWITCH_OUT: 8, __SWITCH_IN: 8}
    }

    def __init__(self):
        self.SWITCH_STATUS = {PCB: {SWITCH: 0 for SWITCH in self.SWITCH_RANGE}
                              for PCB in self.PCB_RANGE}
        self.SWITCH_STATUS[self.PCB_RANGE[0]][self.INPUTS_COMMUTATION[1][self.__CONV1][self.__SWITCH_OUT]] = 0
        self.SWITCH_STATUS[self.PCB_RANGE[0]][self.INPUTS_COMMUTATION[2][self.__CONV2][self.__SWITCH_OUT]] = 0

        self.OUTPUTS_COMMUTATION = dict()
        for board in self.__BS_OUTS:
            for i in range(len(self.__BS_SW_OUT)):
                output = self.__BS_OUTS[board][i]
                self.OUTPUTS_COMMUTATION[output] = {self.__PCB: board, self.__SWITCH_OUT: self.__BS_SW_OUT[i]}

        self.SW_IN_COMMUTATION = {sw_in: {self.__BS_SW_OUT[i]: self.__SW_IN_TO_SW_OUT[sw_in][i]
                                          for i in range(len(self.__BS_SW_OUT))}
                                  for sw_in in self.__SW_IN_TO_SW_OUT}

        self.SW_OUT_COMMUTATION = {sw_out: {self.__BS_SW_IN[i]: self.__SW_OUT_TO_SW_IN[sw_out][i]
                                            for i in range(len(self.__BS_SW_IN))}
                                   for sw_out in self.__SW_OUT_TO_SW_IN}

    # ------------------------------------------------------------------------------------------------------------------
    def auto_to_manual(self, input_, output_):
        _conv = ''
        pack = list()

        if input_ in self.INPUT_CONV_RANGE:
            _conv = self.__CONV1 if input_ == 1 else self.__CONV2
            sw = self.INPUTS_COMMUTATION[input_][_conv][self.__SWITCH_OUT]

            if output_ == 0:
                pass
                # main_psb_conv_switch_config = (self.PCB_RANGE[0], sw, 0)
                # pack.append(main_psb_conv_switch_config)
                # return pack
            elif output_ == 37:
                ch = self.INPUTS_COMMUTATION[input_][_conv][self.__CONV_SWITCH_ON]
                main_psb_conv_switch_config = (self.PCB_RANGE[0], sw, ch)
                pack.append(main_psb_conv_switch_config)
                return pack
            else:
                ch = self.INPUTS_COMMUTATION[input_][_conv][self.__CONV_SWITCH_OFF]
                main_psb_conv_switch_config = (self.PCB_RANGE[0], sw, ch)
                pack.append(main_psb_conv_switch_config)

        b0_sw_out = self.INPUTS_COMMUTATION[input_][self.__SWITCH_OUT]

        if output_ == 0:
            main_psb_output_switch_config = (self.PCB_RANGE[0], b0_sw_out, 0)
        else:
            pcb = self.OUTPUTS_COMMUTATION[output_][self.__PCB]
            b0_sw_out_ch = pcb - 1

            bs_sw_out = self.OUTPUTS_COMMUTATION[output_][self.__SWITCH_OUT]
            bs_sw_in = self.INPUTS_COMMUTATION[input_][self.__SWITCH_IN]

            bs_sw_out_ch = self.SW_OUT_COMMUTATION[bs_sw_out][bs_sw_in]
            bs_sw_in_ch = self.SW_IN_COMMUTATION[bs_sw_in][bs_sw_out]

            psb_input_switch_config = (pcb, bs_sw_in, bs_sw_in_ch)
            psb_output_switch_config = (pcb, bs_sw_out, bs_sw_out_ch)

            pack.append(psb_input_switch_config)
            pack.append(psb_output_switch_config)
            main_psb_output_switch_config = (self.PCB_RANGE[0], b0_sw_out, b0_sw_out_ch)

        pack.append(main_psb_output_switch_config)
        return pack
