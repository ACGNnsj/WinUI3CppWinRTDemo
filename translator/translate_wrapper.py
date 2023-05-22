import argostranslate.package
import argostranslate.translate


def translate(from_code: str, to_code: str, raw_text: str):
    # from_code = "en"
    # to_code = "zh"

    # Translate
    try:
        translated_text = argostranslate.translate.translate(raw_text, from_code, to_code)
    except Exception:
        # Download and install Argos Translate package
        argostranslate.package.update_package_index()
        available_packages = argostranslate.package.get_available_packages()
        # print(available_packages)
        try:
            package_to_install = next(
                filter(
                    lambda x: x.from_code == from_code and x.to_code == to_code, available_packages
                )
            )
        except StopIteration:
            return "Language combination not supported"
        argostranslate.package.install_from_path(package_to_install.download())
        translated_text = argostranslate.translate.translate(raw_text, from_code, to_code)
    # print(translated_text)
    return translated_text
