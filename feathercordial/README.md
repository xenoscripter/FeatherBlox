# FeatherCordial

FeatherCordial is the FeatherBlox integration layer for Cordial.

It is intentionally a small launcher/configuration layer rather than a reimplementation of the Cordial runtime. The upstream Cordial runtime is GPL-3.0-or-later and is maintained separately.

Graphics goal for FeatherBlox hardware: prefer the GLES2/EGL path and do not require Vulkan. Cordial's upstream documentation says Vulkan is dlopen'd as an optional upgrade while GLES2/EGL is the mandatory graphics path.

This layer does not ship Roblox and does not bypass Roblox security.
