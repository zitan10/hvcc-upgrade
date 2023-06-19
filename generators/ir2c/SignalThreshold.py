# Copyright (C) 2014-2018 Enzien Audio, Ltd.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

from HeavyObject import HeavyObject

class SignalThreshold(HeavyObject):

    c_struct = "SignalThreshold"
    preamble = "sThreshold"

    @classmethod
    def get_C_header_set(clazz):
        return {"HvSignalThreshold.h"}

    @classmethod
    def get_C_file_set(clazz):
        return {"HvSignalThreshold.h", "HvSignalThreshold.c"}

    @classmethod
    def get_C_free(clazz, obj_type, obj_id, args):
        return [] # nothing to free

    @classmethod
    def get_C_init(clazz, obj_type, obj_id, args):
        return [
            "{0}_init(this, &{0}_{1}, {2}f, {3}f, {4}f, {5}f);".format(
                clazz.preamble,
                obj_id,
                float(args["triggerThreshold"]),
                float(args["triggerDebounceTime"]),
                float(args["restThreshold"]),
                float(args["restDebounceTime"]))
        ]

    @classmethod
    def get_C_onMessage(clazz, obj_type, obj_id, inlet_index, args):
        return [
            "{0}_onMessage(_c, &Context(_c)->{0}_{1}, {2}, m, NULL);".format(
                clazz.preamble,
                obj_id,
                inlet_index)
        ]

    @classmethod
    def get_C_process(clazz, obj_type, process_dict, args):
        return [
            "{0}_process(this, &{0}_{1}, VIf({2}), &{0}_{1}_sendMessage);".format(
                clazz.preamble,
                process_dict["id"],
                HeavyObject._c_buffer(process_dict["inputBuffers"][0])
            )
        ]
