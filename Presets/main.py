import os
import json


def read_all_preset_files(base_dir: str):
    preset_map = {}
    json_file_list = [os.path.join(base_dir, "CMakePresets.json")]
    while len(json_file_list):
        file_path = os.path.abspath(json_file_list.pop())
        file_dir = os.path.split(file_path)[0]
        with open(file_path, "r", encoding="utf-8") as ifs:
            preset_obj: dict = json.load(ifs)
            preset_map[file_path] = preset_obj
            for include_file_path in preset_obj.get("include", []):
                json_file_list.append(os.path.join(file_dir, include_file_path))
    return preset_map


def combine_presets(preset_map: dict):
    merged_preset = {}
    for _, obj in preset_map.items():
        for key, val in obj.items():
            if key == "include":
                continue

            if key not in merged_preset.keys():
                merged_preset[key] = val
            else:
                if isinstance(val, list):
                    merged_preset[key].extend(val)
                elif isinstance(val, dict):
                    merged_preset[key].update(val)
                else:
                    merged_preset[key] = val

    return merged_preset


def bring_config_to_the_front(preset, target_name):
    config_list: list[dict] = preset["configurePresets"]
    for idx, config in enumerate(config_list):
        if config.get("name", "") == target_name:
            config_list.insert(0, config_list.pop(idx))


preset_map = read_all_preset_files("src/")
merged_preset = combine_presets(preset_map)
bring_config_to_the_front(merged_preset, "config-base")

os.makedirs(".build/", exist_ok=True)
with open(".build/CMakePresets.json", "w", encoding="utf-8") as ofs:
    json.dump(obj=merged_preset, fp=ofs, ensure_ascii=False, indent=2)
