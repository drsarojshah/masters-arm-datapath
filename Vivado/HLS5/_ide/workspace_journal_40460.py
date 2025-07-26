# 2025-05-02T00:52:37.001882200
import vitis

client = vitis.create_client()
client.set_workspace(path="HLS5")

comp = client.get_component(name="Master_Project5_RTL")
comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

comp.run(operation="CO_SIMULATION")

comp.run(operation="PACKAGE")

vitis.dispose()

