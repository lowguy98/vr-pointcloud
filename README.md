<!---
# vr_pointcloud_viewer


## Getting started

To make it easy for you to get started with GitLab, here's a list of recommended next steps.

Already a pro? Just edit this README.md and make it your own. Want to make it easy? [Use the template at the bottom](#editing-this-readme)!

## Add your files

- [ ] [Create](https://docs.gitlab.com/ee/user/project/repository/web_editor.html#create-a-file) or [upload](https://docs.gitlab.com/ee/user/project/repository/web_editor.html#upload-a-file) files
- [ ] [Add files using the command line](https://docs.gitlab.com/ee/gitlab-basics/add-file.html#add-a-file-using-the-command-line) or push an existing Git repository with the following command:

```
```
cd existing_repo
git remote add origin https://gitlab.ifi.uzh.ch/vmml/vr_pointcloud_viewer.git
git branch -M main
git push -uf origin main
```

```
## Integrate with your tools

- [ ] [Set up project integrations](https://gitlab.ifi.uzh.ch/vmml/vr_pointcloud_viewer/-/settings/integrations)

## Collaborate with your team

- [ ] [Invite team members and collaborators](https://docs.gitlab.com/ee/user/project/members/)
- [ ] [Create a new merge request](https://docs.gitlab.com/ee/user/project/merge_requests/creating_merge_requests.html)
- [ ] [Automatically close issues from merge requests](https://docs.gitlab.com/ee/user/project/issues/managing_issues.html#closing-issues-automatically)
- [ ] [Enable merge request approvals](https://docs.gitlab.com/ee/user/project/merge_requests/approvals/)
- [ ] [Automatically merge when pipeline succeeds](https://docs.gitlab.com/ee/user/project/merge_requests/merge_when_pipeline_succeeds.html)

## Test and Deploy

Use the built-in continuous integration in GitLab.

- [ ] [Get started with GitLab CI/CD](https://docs.gitlab.com/ee/ci/quick_start/index.html)
- [ ] [Analyze your code for known vulnerabilities with Static Application Security Testing(SAST)](https://docs.gitlab.com/ee/user/application_security/sast/)
- [ ] [Deploy to Kubernetes, Amazon EC2, or Amazon ECS using Auto Deploy](https://docs.gitlab.com/ee/topics/autodevops/requirements.html)
- [ ] [Use pull-based deployments for improved Kubernetes management](https://docs.gitlab.com/ee/user/clusters/agent/)
- [ ] [Set up protected environments](https://docs.gitlab.com/ee/ci/environments/protected_environments.html)

***

# Editing this README

When you're ready to make this README your own, just edit this file and use the handy template below (or feel free to structure it however you want - this is just a starting point!). Thank you to [makeareadme.com](https://www.makeareadme.com/) for this template.

## Suggestions for a good README
Every project is different, so consider which of these sections apply to yours. The sections used in the template are suggestions for most open source projects. Also keep in mind that while a README can be too long and detailed, too long is better than too short. If you think your README is too long, consider utilizing another form of documentation rather than cutting out information.

## Name
Choose a self-explaining name for your project.

## Description
Let people know what your project can do specifically. Provide context and add a link to any reference visitors might be unfamiliar with. A list of Features or a Background subsection can also be added here. If there are alternatives to your project, this is a good place to list differentiating factors.

## Badges
On some READMEs, you may see small images that convey metadata, such as whether or not all the tests are passing for the project. You can use Shields to add some to your README. Many services also have instructions for adding a badge.

## Visuals
Depending on what you are making, it can be a good idea to include screenshots or even a video (you'll frequently see GIFs rather than actual videos). Tools like ttygif can help, but check out Asciinema for a more sophisticated method.

## Installation
Within a particular ecosystem, there may be a common way of installing things, such as using Yarn, NuGet, or Homebrew. However, consider the possibility that whoever is reading your README is a novice and would like more guidance. Listing specific steps helps remove ambiguity and gets people to using your project as quickly as possible. If it only runs in a specific context like a particular programming language version or operating system or has dependencies that have to be installed manually, also add a Requirements subsection.

## Usage
Use examples liberally, and show the expected output if you can. It's helpful to have inline the smallest example of usage that you can demonstrate, while providing links to more sophisticated examples if they are too long to reasonably include in the README.

## Support
Tell people where they can go to for help. It can be any combination of an issue tracker, a chat room, an email address, etc.

## Roadmap
If you have ideas for releases in the future, it is a good idea to list them in the README.

## Contributing
State if you are open to contributions and what your requirements are for accepting them.

For people who want to make changes to your project, it's helpful to have some documentation on how to get started. Perhaps there is a script that they should run or some environment variables that they need to set. Make these steps explicit. These instructions could also be useful to your future self.

You can also document commands to lint the code or run tests. These steps help to ensure high code quality and reduce the likelihood that the changes inadvertently break something. Having instructions for running tests is especially helpful if it requires external setup, such as starting a Selenium server for testing in a browser.

## Authors and acknowledgment
Show your appreciation to those who have contributed to the project.

## License
For open source projects, say how it is licensed.

## Project status
If you have run out of energy or time for your project, put a note at the top of the README saying that development has slowed down or stopped completely. Someone may choose to fork your project or volunteer to step in as a maintainer or owner, allowing your project to keep going. You can also make an explicit request for maintainers.

-->
# 3D point cloud VR Viewer
The 3D point cloud VR Viewer can be used to visualize 3D point cloud data representing a room setting in a virtual environment. As an input format, the viewer accepts a PTX-file format. It works with a HMD-device, controllers and at least one base station from HTC. 

Interactions with the visualized 3D point cloud data are possible through controller input. The user is able to jump/hover within the 3D point cloud room on a xy-plane. The interactions are bounded by the walls of the 3D point cloud room. Next to that, it is possible to stop the visualization by pressing one of the trigger buttons on the controller. 

## Setup and requirements
To successfully run the project, a HTC HMD-device and a base station is required. With that, the computer you want to run the project on has to fulfill some minimum requirements to sucessfully run a VR (SteamVR provides a performance test of their device for potential VR-users: https://store.steampowered.com/app/323910/SteamVR_Performance_Test/ ). Additionally, to interact with the 3D point cloud data in the virtual environment, controllers (HTC Vive required) are needed to make use of the interaction possibilities that are included in the project. 
 
We used CMake (at least version 3.17 is required), Visual Studio 2019 (any later version should also work), SteamVR and Windows 11 to build and run the project. The different external libraries the project requires are already integrated in the project folder. Therefore, no further actions are necessary in that regard.  

## Build and Run 
3D Point Cloud VR Viewer is a cross-platform project based on CMake. To compile and run the 3D point cloud VR Viewer, please use CMake to compile the project.
Before running, please change the file location for reading PTX data and action manifest location in the code. The file location of PTX data can be specified in main.cpp and the file location of action manifest can be specified in app.cpp.


## Roadmap
The presented project, the 3D point cloud VR Viewer, is a result of a master project at UZH (Visualization and Multimedia Lab). In the future many improvements can be added to the project. Below you find a list of suggestions on possible improvements: 
- Dynamically align the coordinate system of the virtual environment world with the model coordinate system ensuring that the boundary functions work at any time (adjusting the rotation matrix accordingly).
- Additional functionality of triangulation respectively mesh creation to be able to switch between the two modes. 
- Improve interface to get input parameters from user. 
- Add functionality to process multiple PTX-files to close the gaps of no points in the point cloud visualization. 
- Add functionality to process different file formats

## Authors and acknowledgment
The 3D point cloud VR Viewer is a master project of Sarah Feuz and Jie Liao at the Visualization and Multimedia Lab (University of Zurich). Many thanks to Luciano Arnaldo Romero Calla for supporting us during the process of building the 3D point cloud VR Viewer and to Prof. Dr. Renato Pajarola, who supervised the project. 



