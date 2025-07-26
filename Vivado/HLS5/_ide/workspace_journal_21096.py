# 2025-05-01T14:00:23.359967200
import vitis

client = vitis.create_client()
client.set_workspace(path="HLS5")

comp = client.get_component(name="Master_Project5_RTL")
comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

comp.run(operation="CO_SIMULATION")

comp.run(operation="PACKAGE")

vitis.dispose()

