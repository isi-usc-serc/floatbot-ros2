from setuptools import find_packages, setup

package_name = 'floatbot_mpc'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='dbacher',
    maintainer_email='dbacher@usc.edu',
    description='Floatbot MPC controller',
    license='MIT',
    extras_require={
        'test': [
            'pytest',
        ],
    },
    entry_points={
        'console_scripts': [
            'floatbot_mpc = floatbot_mpc.floatbot_mpc:main'
        ],
    },
)
