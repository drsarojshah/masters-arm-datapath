# 2025-04-29T19:53:36.221907800
import vitis

client = vitis.create_client()
client.set_workspace(path="HLS5")

comp = client.create_hls_component(name = "Master_project6_RTL",cfg_file = ["hls_config.cfg"],template = "empty_hls_component")

comp = client.get_component(name="Master_project6_RTL")
comp.run(operation="C_SIMULATION")

client.delete_component(name="Master_project6_RTL")

comp = client.create_hls_component(name = "Master_Project5_RTL",cfg_file = ["hls_config.cfg"],template = "empty_hls_component")

comp = client.get_component(name="Master_Project5_RTL")
comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

